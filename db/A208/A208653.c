/*
 * A208653  a(n) = Product_{i=floor((n + 1)/2)..n-1} binomial(n-1, i).
 * https://oeis.org/A208653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=floor((n+1)/2)}^{n-1} binomial(n-1,i)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                                // empty product

    idx_t m = n - 1;
    idx_t start = (n + 1) >> 1;                  // floor((n+1)/2)

    seq_t prod = 1;
    for (idx_t i = start; i <= m; ++i)
        prod *= binomial(m, i);                  // multiply each binomial

    return prod;
}
