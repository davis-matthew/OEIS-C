/*
 * A042750  Numerators of continued fraction convergents to sqrt(906).
 * https://oeis.org/A042750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 602*a(n-2) - a(n-4), for n≥4 Initial values: a(0)=30,
 *            a(1)=301, a(2)=18090, a(3)=181201
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Ring buffer of the last four terms */
    seq_t buf[4];
    buf[0] = 30;
    buf[1] = 301;
    buf[2] = 18090;
    buf[3] = 181201;

    if (n < 4) {
        return buf[(size_t)n];
    }

    /* Compute terms iteratively up to n */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        /* a(i) = 602*a(i-2) - a(i-4) */
        seq_t val = (seq_t)602 * buf[(i + 2) & 3] - buf[i & 3];
        buf[i & 3] = val;
    }

    return buf[(size_t)n & 3];
}
