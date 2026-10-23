/*
 * A126446  Column 0 of triangle A126445; a(n) = binomial( binomial(n+2,3),
 *          n).
 * https://oeis.org/A126446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( binomial(n+2,3), n )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute k = binomial(n+2, 3) = (n)(n+1)(n+2)/6 as an idx_t. */
    unsigned __int128 prod = (unsigned __int128)n * (n + 1) * (n + 2);
    idx_t k = (idx_t)(prod / 6);

    /* Return binomial(k, n).  binom works for any idx_t arguments. */
    return binom(k, n);
}
