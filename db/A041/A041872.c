/*
 * A041872  Numerators of continued fraction convergents to sqrt(458).
 * https://oeis.org/A041872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 214*a(n-3) + a(n-6) for n > 5 with a(0)=21, a(1)=43,
 *            a(2)=107, a(3)=4537, a(4)=9181, a(5)=22899
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    if (n < 6) {
        static const seq_t init[6] = {21, 43, 107, 4537, 9181, 22899};
        return init[n];
    }

    /* Use a ring buffer of length 6 to hold the last 6 values. */
    seq_t buf[6] = {21, 43, 107, 4537, 9181, 22899};
    idx_t i;

    /* Compute terms from 6 up to n. */
    for (i = 6; i <= n; ++i) {
        /* a(i) = 214*a(i-3) + a(i-6) */
        seq_t val = (seq_t)214 * buf[(i - 3) % 6] + buf[(i - 6) % 6];
        buf[i % 6] = val;
    }

    return buf[n % 6];
}
