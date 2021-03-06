/*
 * Copyright (c) 2019 Analog Devices Inc.
 *
 * This file is part of Scopy
 * (see http://www.github.com/analogdevicesinc/scopy).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
/* -*- c++ -*- */
/*
 * Copyright 2008,2009 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file LICENSE.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "utils.h"
#include <QDebug>
#include <QSizePolicy>

QwtPickerDblClickPointMachine::QwtPickerDblClickPointMachine()
#if QWT_VERSION < 0x060000
    : QwtPickerMachine ()
#else
    : QwtPickerMachine (PointSelection)
#endif
{
}

QwtPickerDblClickPointMachine::~QwtPickerDblClickPointMachine()
{
}

#if QWT_VERSION < 0x060000
#define CMDLIST_TYPE QwtPickerMachine::CommandList
#else
#define CMDLIST_TYPE QList<QwtPickerMachine::Command>
#endif
CMDLIST_TYPE
QwtPickerDblClickPointMachine::transition(const QwtEventPattern &eventPattern,
					  const QEvent *e)
{
  CMDLIST_TYPE cmdList;
  switch(e->type()) {
    case QEvent::MouseButtonDblClick:
      if ( eventPattern.mouseMatch(QwtEventPattern::MouseSelect1,
				   (const QMouseEvent *)e) ) {
	cmdList += QwtPickerMachine::Begin;
	cmdList += QwtPickerMachine::Append;
	cmdList += QwtPickerMachine::End;
      }
      break;
  default:
    break;
  }
  return cmdList;
}

#if QWT_VERSION < 0x060100
QwtDblClickPlotPicker::QwtDblClickPlotPicker(QwtPlotCanvas* canvas)
#else /* QWT_VERSION < 0x060100 */
QwtDblClickPlotPicker::QwtDblClickPlotPicker(QWidget* canvas)
#endif /* QWT_VERSION < 0x060100 */
  : QwtPlotPicker(canvas)
{
#if QWT_VERSION < 0x060000
  setSelectionFlags(QwtPicker::PointSelection);
#endif
}

QwtDblClickPlotPicker::~QwtDblClickPlotPicker()
{
}

QwtPickerMachine*
QwtDblClickPlotPicker::stateMachine(int n) const
{
  return new QwtPickerDblClickPointMachine;
}

void Util::retainWidgetSizeWhenHidden(QWidget *w, bool retain)
{
	QSizePolicy sp_retain = w->sizePolicy();
	sp_retain.setRetainSizeWhenHidden(retain);
	w->setSizePolicy(sp_retain);
}

void Util::setWidgetNrOfChars(QWidget *w,
		int minNrOfChars, int maxNrOfChars)
{
	QFontMetrics labelm(w->font());

	auto label_min_width = labelm.width(QString(minNrOfChars,'X'));
	w->setMinimumWidth(label_min_width);

	if (maxNrOfChars!=0) {
		auto label_max_width = labelm.width(QString(maxNrOfChars,'X'));
		w->setMaximumWidth(label_max_width);
	}
}

QString Util::loadStylesheetFromFile(const QString &path)
{
	QFile file(path);
	file.open(QFile::ReadOnly);
	QString stylesheet = QString::fromLatin1(file.readAll());
	return stylesheet;
}

