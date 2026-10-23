/*
 * A041465  Denominators of continued fraction convergents to sqrt(248).
 * https://oeis.org/A041465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 126*a(n-4) - a(n-8) for n > 7 Initial values: a(0) = 1,
 *            a(1) = 1, a(2) = 3, a(3) = 4, a(4) = 123, a(5) = 127, a(6) =
 *            377, a(7) = 504
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* buffer holding the last 8 computed values */
    static seq_t buf[8];

    /* initialize the first 8 terms on every call */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)1;
    buf[2] = (seq_t)3;
    buf[3] = (seq_t)4;
    buf[4] = (seq_t)123;
    buf[5] = (seq_t)127;
    buf[6] = (seq_t)377;
    buf[7] = (seq_t)504;

    /* return early if the requested index is within the initial range */
    if (n < 8)
        return buf[(int)(n & 7ULL)];  /* n is uint64_t */

    /* compute sequentially using the recurrence */
    idx_t i;
    for (i = 8; i <= n; ++i) {
        seq_t term4 = buf[(i - 4) & 7ULL];
        seq_t term8 = buf[(i - 8) & 7ULL];
        buf[i & 7ULL] = (seq_t)126 * term4 - term8;
    }

    return buf[n & 7ULL];
}
