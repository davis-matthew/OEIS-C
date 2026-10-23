/*
 * A171471  a(n) = 6*a(n-1)-8*a(n-2)-9 for n > 2; a(0) = 35, a(1) = 225, a(2)
 *          = 837.
 * https://oeis.org/A171471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*4^n + 18*2^n - 3 for n > 0; a(0) = 35
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"


seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)35;
    }
    seq_t term1 = (seq_t)48 * pow2(2 * (idx_t)n);  /* 48·4^n = 48·2^(2n) */
    seq_t term2 = (seq_t)18 * pow2((idx_t)n);      /* 18·2^n */
    return term1 + term2 - (seq_t)3;
}
