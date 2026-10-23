/*
 * A014115  Order of a certain Clifford group in dimension 2^n (the
 *          automorphism group of the Barnes-Wall lattice for n != 3).
 * https://oeis.org/A014115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2+n+1) * (2^n-1) * ∏_{i=1}^{n-1} (2^(2*i)-1) with
 *            special case a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    /* Compute 2^(n^2+n+1) */
    idx_t exp = (idx_t)(((seq_t)n * (seq_t)n) + (seq_t)n + 1);
    seq_t result = pow2(exp);

    /* Multiply by (2^n - 1) */
    seq_t twon = pow2(n);
    result *= (twon - (seq_t)1);

    /* Multiply by the product for i = 1..n-1 of (2^(2*i) - 1) */
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t term = pow2((idx_t)(2 * i));
        result *= (term - (seq_t)1);
    }

    return result;
}
