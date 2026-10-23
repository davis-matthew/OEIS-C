/*
 * A133707  Denominator of a remarkable product.
 * https://oeis.org/A133707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of * ∏_{i=1}^{n-1} (i^4 – 15·i^3 + 85·i^2 –
 *            225·i + 394) / 120 * The denominator is a power of 5; this
 *            function returns 5^k
 * Range:     u8 n <= 4475, u16 n <= 8162, u32 n <= 8162, u64 n <= 8162,
 *            u128 n <= 8162, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Count the exponent of 5 in a 64‑bit integer. */
static inline int v5_of_uint64(uint64_t x)
{
    int v = 0;
    while ((x % 5ULL) == 0ULL) {
        ++v;
        x /= 5ULL;
    }
    return v;
}

/* Cumulative sums of exponents of 5 in the numerators N_i. */
static idx_t *cum = NULL;        /* cum[i] = Σ_{k=1..i} v5(N_k)   */
static idx_t cum_size = 0;       /* size ≥ i+1 for ready indices */

/* Ensure the cumulative array has entries up to index 'up' (inclusive). */
static void ensure_cum(idx_t up)
{
    if (cum_size == 0) {                    /* first allocation */
        cum = (idx_t *)malloc((up + 1) * sizeof(idx_t));
        if (!cum) abort();
        cum[0] = 0;
        cum_size = 1;                       /* cum[0] represents i=0   */
    }
    if (cum_size <= up) {                   /* extend the array       */
        idx_t old = cum_size;
        cum = (idx_t *)realloc(cum, (up + 1) * sizeof(idx_t));
        if (!cum) abort();
        for (idx_t i = old; i <= up; ++i) {
            uint64_t ii = i;
            uint64_t N = ii * ii * ii * ii
                        - 15 * ii * ii * ii
                        + 85 * ii * ii
                        - 225 * ii
                        + 394;
            int v5 = v5_of_uint64(N);
            cum[i] = cum[i - 1] + (idx_t)v5;
        }
        cum_size = up + 1;
    }
}

seq_t a(idx_t n)
{
    /* According to OEIS, a(1) is defined.  For safety, handle n=0. */
    if (n == 0) return (seq_t)1;

    /* All terms i≥1 contribute one factor 5 from the denominator 120=2^3·3·5. */
    idx_t denom_v5 = n - 1;   /* number of factors of 5 in the denominators   */

    /* Compute the total exponent of 5 in the numerators up to i=n-1. */
    ensure_cum(n - 1);
    idx_t numer_v5 = cum[n - 1];

    /* The remaining exponent of 5 contributes to the final denominator. */
    idx_t exp = 0;
    if (denom_v5 > numer_v5)
        exp = denom_v5 - numer_v5;

    /* Compute 5^exp (big_pow is provided by the utils library). */
    return big_pow((seq_t)5, (seq_t)exp);
}
