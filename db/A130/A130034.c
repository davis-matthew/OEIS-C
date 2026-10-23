/*
 * A130034  Denominators of partial sums of a series for the inverse of the
 *          arithmetic-geometric mean (agM) of 1 and sqrt(2)/2.
 * https://oeis.org/A130034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2^(5*n - 2*popcount(n)) (OEIS A130034)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* popcount of a 64‑bit integer – safe because idx_t is uint64_t */
    idx_t pop = __builtin_popcountll(n);

    /* exponent of 2 in the denominator */
    idx_t exp = 5 * n - 2 * pop;

    return pow2(exp);
}
