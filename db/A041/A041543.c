/*
 * A041543  Denominators of continued fraction convergents to sqrt(288).
 * https://oeis.org/A041543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 34*a(n-2) - a(n-4) for n >= 4 * Initial terms: a(0)=1,
 *            a(1)=1, a(2)=33, a(3)=34 * This recurrence produces the
 *            denominators of the convergents to sqrt(288)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)33;
    if (n == 3)
        return (seq_t)34;

    /* Circular buffer of the last four values: a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)1;   /* a(1) */
    buf[2] = (seq_t)33;  /* a(2) */
    buf[3] = (seq_t)34;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 34 * a(i-2) - a(i-4) */
        seq_t val = (seq_t)34 * buf[(i - 2) & 3u] - buf[(i - 4) & 3u];
        buf[i & 3u] = val;
    }

    return buf[n & 3u];
}
