/*
 * A089985  a(n) = A089709(n+1)/A089709(n).
 * https://oeis.org/A089985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(2) = 3, a(n) = 2^(n-1) + 2 for n >= 3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term. */
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe default */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    /* For n >= 3 use the explicit formula 2^(n-1)+2. */
    return pow2(n - 1) + (seq_t)2;
}
