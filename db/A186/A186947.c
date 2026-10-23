/*
 * A186947  a(n) = 4^n - n*2^n.
 * https://oeis.org/A186947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - n*2^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pow2_n = pow2(n);                 /* 2^n                       */
    seq_t pow4_n = pow2(n << 1);            /* 4^n (i.e. 2^(2n))         */
    seq_t n_mul  = mul_small(pow2_n, (unsigned int)n); /* n*2^n   */
    return pow4_n - n_mul;                  /* 4^n - n*2^n               */
}
