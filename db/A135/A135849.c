/*
 * A135849  a(n) is the ratio of the sum of the bends (curvatures) of the
 *          circles in the n-th generation of an Apollonian packing to the sum
 *          of the bends in the initial four-circle configuration.
 * https://oeis.org/A135849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 3*a(n-2) with initial values a(1)=1, a(2)=5,
 *            a(3)=39
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the supplied helper mul_small
   (../utils/mul_small.h) to avoid overflow when multiplying by
   small constants. */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)39;

    seq_t prev2 = (seq_t)5;   /* a(2) */
    seq_t prev1 = (seq_t)39;  /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = mul_small(prev1, 8);   /* 8*a(i-1) */
        seq_t term2 = mul_small(prev2, 3);   /* 3*a(i-2) */
        cur = term1 - term2;                /* a(i) = 8*a(i-1) - 3*a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
