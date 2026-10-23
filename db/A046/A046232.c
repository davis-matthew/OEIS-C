/*
 * A046232  Cubes which are palindromes in base 4.
 * https://oeis.org/A046232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 for n = 1, 1 for n = 2; for n > 2, a(n) = 64^(n-2) +
 *            3*16^(n-2) + 3*4^(n-2) + 1 (OEIS A046232)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    idx_t k = n - 2;                   /* exponent for the powers */

    seq_t pow4  = pow_uint(4ULL,   k);
    seq_t pow16 = pow_uint(16ULL,  k);
    seq_t pow64 = pow_uint(64ULL,  k);

    return pow64 + (seq_t)3 * pow16 + (seq_t)3 * pow4 + (seq_t)1;
}
