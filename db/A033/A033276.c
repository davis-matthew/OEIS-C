/*
 * A033276  Number of diagonal dissections of an n-gon into 4 regions.
 * https://oeis.org/A033276
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,3)*binomial(n-3,3)/4
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 92, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 5 the sequence is undefined; returning 0 keeps the function
       well‑defined for all n >= 0. */
    if (n < 5)
        return (seq_t)0;

    /* Compute the two binomial coefficients and their product. */
    seq_t b1 = binomial(n + 2, 3u);
    seq_t b2 = binomial(n - 3, 3u);

    seq_t res = b1 * b2;
    res /= 4u;          /* exact division by 4 */
    return res;
}
