/*
 * A126451  Column 0 of triangle A126450; a(n) = C( C(n+2,3) + 1, n).
 * https://oeis.org/A126451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( binomial(n+2,3) + 1 , n )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   binom, binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* C(n+2,3) as a seq_t */
    seq_t inner = binom_n_k(n + 2, 3);
    /* N = inner + 1, fit into idx_t for our range */
    idx_t N = (idx_t)(inner + (seq_t)1);
    /* Return C(N, n) */
    return binom(N, n);
}
