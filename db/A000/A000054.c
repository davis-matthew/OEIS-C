/*
 * A000054  Local stops on New York City A line subway.
 * https://oeis.org/A000054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   none exists. A000054 is a finite ("fini, full") list of
 *            real-world data: the numbered street stops of the NYC A train
 *            (Feb 2000), from 4th St up to Inwood-207th St, 25 terms in all.
 *            The street numbers follow from where the stations were built,
 *            not from any rule, so the table below is the definiti...
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned short a000054_stops[] = {
    4, 14, 23, 34, 42, 50, 59, 72, 81, 86, 96, 103, 110, 116, 125,
    135, 145, 155, 163, 168, 175, 181, 190, 200, 207
};

seq_t a(idx_t n)
{
    idx_t len = (idx_t)(sizeof(a000054_stops) / sizeof(a000054_stops[0]));
    if (n < 1 || n > len)
        return (seq_t)0;
    return (seq_t)a000054_stops[n - 1];
}
