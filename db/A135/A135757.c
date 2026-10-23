/*
 * A135757  Central binomial coefficients at triangular positions: a(n) =
 *          A000984(n(n+1)/2).
 * https://oeis.org/A135757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n*(n+1), n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute 2*m = n*(n+1) in a large integer to avoid overflow. */
    seq_t twice = (seq_t)n * ((seq_t)n + 1);
    /* m = n*(n+1)/2 fits in idx_t for all supported n. */
    idx_t m = (idx_t)(twice / 2);
    /* Return binomial(2*m, m).  binom() accepts idx_t arguments. */
    return binom((idx_t)twice, m);
}
