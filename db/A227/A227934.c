/*
 * A227934  G.f.: Sum_{n>=0} x^n / (1-x)^(n^4).
 * https://oeis.org/A227934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k^4 + n - k - 1, n - k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1 */
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute k^4 safely in idx_t (uint64_t).  For n up to about 10000, k^4 < 2^64. */
        idx_t k4 = k * k * k * k;
        idx_t n_minus_k = n - k;
        /* a_val = k^4 + n - k - 1 ; non‑negative for all k in [0,n] when n>0 */
        idx_t a_val = k4 + n_minus_k - 1;
        sum += binomial(a_val, n_minus_k);
    }
    return sum;
}
