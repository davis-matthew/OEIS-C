/*
 * A235883  Number of (n+1) X (7+1) 0..2 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(1932 + 127*2^(1+n) + 3^n) (offset 1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the entry for seq_pow from ../utils/seq_pow.h. */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* For indices below the offset the sequence is not defined; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^(n+1) and 3^n using the generic exponentiation routine. */
    seq_t two_pow  = seq_pow((seq_t)2, n + 1);
    seq_t three_pow = seq_pow((seq_t)3, n);

    /* Compute the formula: 3*(1932 + 127*2^(1+n) + 3^n) */
    seq_t part = (seq_t)1932 + ((seq_t)127 * two_pow) + three_pow;
    return (seq_t)3 * part;
}
