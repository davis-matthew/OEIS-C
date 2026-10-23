/*
 * A132727  a(n) = 3 * 2^(n-1) * a(n-1) with a(0) = 1.
 * https://oeis.org/A132727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * 2^(n*(n-1)/2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for all n >= 0, with a(0)=1.
 * Uses the general power functions from the utilities. */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Exponent for the 2 term: n*(n-1)/2 */
    idx_t bin_exp = n * (n - 1) / 2;

    /* 3^n */
    seq_t pow3 = seq_pow((seq_t)3, n);

    /* 2^bin_exp */
    seq_t pow2 = seq_pow((seq_t)2, bin_exp);

    /* Multiply the two powers */
    return seq_mul(pow3, pow2);
}
