/*
 * A235888  Number of (n+1) X (3+1) 0..3 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * (42 + 21 * 2^n + 4^n) (OEIS A235888)
 * Range:     u8 none, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and 4^n using the provided big_pow helper. */
    seq_t two_pow   = big_pow((seq_t)2, (seq_t)n);
    seq_t four_pow  = big_pow((seq_t)4, (seq_t)n);

    /* Sum the terms: 42 + 21*2^n + 4^n */
    seq_t sum  = (seq_t)42 + ((seq_t)21 * two_pow) + four_pow;

    /* Final result: 4 * (sum) */
    return (seq_t)4 * sum;
}
