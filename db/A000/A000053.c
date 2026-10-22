/*
 * A000053  Local stops on New York City 1 Train (Broadway-7 Avenue Local)
 *          subway.
 * https://oeis.org/A000053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   none exists. A000053 is real-world data: the numbered stops of
 *            the NYC 1 train as of February 2000 (fini, full; 29 terms). It
 *            cannot be computed from a rule. The table stores the gaps
 *            between consecutive stops, a(1) = 14 and a(n) = a(n-1) + gap(n),
 *            but that is just the same data in another form....
 * Range:     u8 n <= 29, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned char a000053_gaps[] = {
    14, 4, 5, 5, 6, 8, 8, 9, 7, 6, 7, 7, 10, 7, 7,
    6, 9, 12, 8, 12, 11, 13, 10, 16, 8, 10, 6, 7, 4
};

seq_t a(idx_t n)
{
    idx_t count = (idx_t)(sizeof(a000053_gaps) / sizeof(a000053_gaps[0]));
    seq_t s = 0;
    idx_t i;
    if (n < 1 || n > count)
        return (seq_t)0;
    for (i = 0; i < n; i++)
        s += (seq_t)a000053_gaps[i];
    return s;
}
