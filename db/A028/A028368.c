/*
 * A028368  a(n) = (Product_{j=1..n-1} (2^j-1)) * 2^binomial(n+1,2).
 * https://oeis.org/A028368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{j=1}^{n-1} (2^j-1)) * 2^(n(n+1)/2), offset 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0U)
        return (seq_t)1;

    /* Compute 2^(n(n+1)/2) */
    idx_t exp = (n + 1U) * n / 2U;          /* fits in idx_t for all n */
    seq_t pow_factor = pow2(exp);           /* power of two from utils */

    /* Compute ∏_{j=1}^{n-1} (2^j-1) iteratively */
    seq_t prod = (seq_t)1;
    seq_t pow2_j = (seq_t)2;                /* 2^1 */

    for (idx_t j = 1U; j < n; ++j) {
        prod *= (pow2_j - (seq_t)1);
        pow2_j *= (seq_t)2;                /* advance to 2^(j+1) */
    }

    return prod * pow_factor;
}
