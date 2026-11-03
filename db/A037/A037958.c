/*
 * A037958  a(n) = binomial(n, floor((n-8)/2)).
 * https://oeis.org/A037958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor((n-8)/2))
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 3180 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* For n < 8 the binomial coefficient is zero. */
    if (n < 8)
        return 0;

    /* Compute k = floor((n-8)/2) using unsigned arithmetic. */
    idx_t k = (n - 8) / 2;

    /* Use the symmetry k ↦ n-k to minimise the work. */
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Current factors of the binomial product. */
        seq_t numer = (seq_t)(n - k + i);  /* (n-k+i) */
        seq_t denom = (seq_t)i;           /* i          */

        /* Cancel common factors between numer and denom. */
        seq_t g = gcd(denom, numer);
        denom /= g;
        numer /= g;

        /* Whatever factor of denom still divides result can be removed
           before the multiplication to keep intermediate values small. */
        g = gcd(result, denom);
        result /= g;
        denom /= g;   /* denom should be 1 now */

        /* Multiply the reduced numerator into the result.  numer is
           at most n (≤ 10 000), so we can safely use mul_small. */
        result = mul_small(result, (unsigned int)numer);
    }

    return result;
}
