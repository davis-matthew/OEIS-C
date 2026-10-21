/*
 * A217652  Number of isolated nodes over all labeled directed graphs on n
 *          nodes.
 * https://oeis.org/A217652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * 2^(n^2 - 3*n + 2) (OEIS A217652)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The exponent e = n^2 - 3*n + 2 fits into idx_t for all practical n (≤ 10000). */
    idx_t exp = n * n - 3 * n + 2;
    /* Compute 2^exp using the provided pow2 helper. */
    seq_t pow = pow2(exp);
    /* Multiply by n. Multiplication by 0 yields 0 automatically. */
    return (seq_t)n * pow;
}
