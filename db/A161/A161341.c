/*
 * A161341  First differences of A161340.
 * https://oeis.org/A161341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 56 * 7^(popcount(n-1)-1) for n > 1 a(1) = 1 If n > 1 and
 *            n is a power of 2, subtract 3*n*(3*n-1) from the above value
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* popcount of n-1 (n > 1 guarantees n-1 > 0) */
    int pop = 0;
    for (idx_t t = n - 1; t; t >>= 1)
        pop += t & 1U;

    /* compute 7^(pop-1) */
    seq_t pow7 = 1;
    for (int i = 0; i < pop - 1; ++i)
        pow7 *= (seq_t)7;

    /* base value: 56 * pow7 */
    seq_t val = pow7 * (seq_t)56;

    /* apply subtraction for powers of two (excluding n=1) */
    if ((n & (n - 1)) == 0) {
        seq_t tmp = (seq_t)3 * n;          /* tmp = 3*n */
        val -= tmp * (tmp - (seq_t)1);     /* subtract 3*n*(3*n-1) */
    }

    return val;
}
