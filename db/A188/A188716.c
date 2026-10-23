/*
 * A188716  a(n) = n + (n-1)*(2^n-2).
 * https://oeis.org/A188716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n-1)*(2^n-2)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    seq_t n_val      = (seq_t)n;
    seq_t n_minus_1  = (seq_t)(n - (idx_t)1);
    seq_t pow2_n     = pow2(n);
    seq_t prod1      = n_minus_1 * pow2_n;
    seq_t prod2      = n_minus_1 * ((seq_t)2);
    return n_val + prod1 - prod2;
}
