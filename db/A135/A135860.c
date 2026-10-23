/*
 * A135860  a(n) = binomial(n*(n+1), n).
 * https://oeis.org/A135860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n*(n+1), n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Computes a(n) = binomial(n*(n+1), n) for n >= 0. */
seq_t a(idx_t n)
{
    /* Compute the upper argument as seq_t to avoid overflow in the
       multiplication before truncation to idx_t. */
    idx_t upper = (idx_t)n * ((idx_t)n + 1);
    return binomial(upper, n);
}
