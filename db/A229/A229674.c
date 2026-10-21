/*
 * A229674  a(n) = Sum_{k = 0..n} Product_{j = 0..6} C(n+j*k,k).
 * https://oeis.org/A229674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} Product_{j = 0..6} C(n + j*k, k) where
 *            C(m, k) is the binomial coefficient. Uses the binomial helper
 *            function from "utils/binomial.h"
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 1;                        /* term for k = 0 */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term = 1;
        for (idx_t j = 0; j < 7; ++j)
            term *= binomial(n + j * k, k);
        sum += term;
    }
    return sum;
}
