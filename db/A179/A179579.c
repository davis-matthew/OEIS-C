/*
 * A179579  Number of permutations of {1, ..., n+4} with p(i) < i at least n
 *          times more often than p(i) > i.
 * https://oeis.org/A179579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+4)*(n+1))/2 + 2^(n+4)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Cast index to seq_t to avoid overflow in intermediate arithmetic */
    seq_t nn = (seq_t)n;

    /* Triangular contribution: (n+4)*(n+1)/2 */
    seq_t tri = (nn + (seq_t)4u) * (nn + (seq_t)1u) / (seq_t)2u;

    /* Exponential contribution: 2^(n+4) using the pow2 helper */
    seq_t exp = pow2(n + (idx_t)4u);

    return tri + exp;
}
