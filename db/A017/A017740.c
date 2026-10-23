/*
 * A017740  Binomial coefficients C(n,76).
 * https://oeis.org/A017740
 *
 * Offset:    76    (a(76) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,76) for n >= 76 (0 otherwise)
 * Range:     u8 n <= 77, u16 n <= 78, u32 n <= 83, u64 n <= 94, u128 n <= 133,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_gcd_idx.h"

/* Euclidean GCD for idx_t (uint64_t) */

seq_t a(idx_t n)
{
    const idx_t k = 76;
    if (n < k) return 0;          /* offset handling */

    /* numerator terms: n-75, n-74, …, n */
    idx_t numer[k];
    for (idx_t i = 0; i < k; ++i)
        numer[i] = n - k + 1 + i; /* n-75 + i */

    /* denominator terms: 1, 2, …, 76 */
    idx_t denom[k];
    for (idx_t i = 0; i < k; ++i)
        denom[i] = i + 1;

    /* Reduce using GCD to avoid overflow */
    for (idx_t i = 0; i < k; ++i)
    {
        if (numer[i] == 1) continue;
        for (idx_t j = 0; j < k && numer[i] > 1; ++j)
        {
            if (denom[j] == 1) continue;
            idx_t g = _gcd_idx(numer[i], denom[j]);
            if (g > 1)
            {
                numer[i] /= g;
                denom[j] /= g;
            }
        }
    }

    /* Multiply remaining numerators into the result */
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i)
    {
        if (numer[i] > 1)
            res *= (seq_t)numer[i];
    }
    return res;
}
