/*
 * A081201  7th binomial transform of (0,1,0,1,0,1,....), A000035.
 * https://oeis.org/A081201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(8^n - 6^n). It also satisfies the linear
 *            recurrence a(n) = 14*a(n-1) - 48*a(n-2) with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(n) = 14*a(n-1) - 48*a(n-2) */
        seq_t t1 = prev1 * (seq_t)14;
        seq_t t2 = prev2 * (seq_t)48;
        cur = t1 - t2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
