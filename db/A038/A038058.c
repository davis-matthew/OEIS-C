/*
 * A038058  Number of labeled trees with 2-colored nodes.
 * https://oeis.org/A038058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * n^(n-2) for n>=1; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   pow_uint_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint_n.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    seq_t two_pow = pow_uint_n((seq_t)2, n);
    seq_t n_pow  = pow_uint_n((seq_t)n, n - (idx_t)2);
    return two_pow * n_pow;
}
