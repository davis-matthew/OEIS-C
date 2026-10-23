/*
 * A022649  Expansion of Product_{m >=1} (1+m*q^m)^21.
 * https://oeis.org/A022649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 30, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A022649: Coefficients of ∏_{m≥1} (1 + m q^m)^21
   Formula: a(n) = coefficient of q^n in ∏_{m≥1} (1 + m q^m)^21
   The implementation recomputes the full table up to the largest n requested.
   It uses a dynamic programming approach where for each m we convolve the
   current polynomial with (1 + m q^m)^21, which has at most 22 terms.
*/

#include <stdlib.h>

/* precomputed binomial coefficients C(21, t) */
static const seq_t bin[22] = {
    (seq_t)1,     (seq_t)21,    (seq_t)210,    (seq_t)1330,
    (seq_t)5985,  (seq_t)20349, (seq_t)54264,  (seq_t)116280,
    (seq_t)203490,(seq_t)293930,(seq_t)352716,(seq_t)352716,
    (seq_t)293930,(seq_t)203490,(seq_t)116280,(seq_t)54264,
    (seq_t)20349,(seq_t)5985,  (seq_t)1330,   (seq_t)210,
    (seq_t)21,    (seq_t)1
};

/* static cache of a(0)..a(max_n) */
static seq_t *cache = NULL;      /* array of seq_t */
static idx_t  max_n  = 0;        /* highest n computed */

/* helper to allocate or extend the cache */
static void grow_cache(idx_t n)
{
    if (n <= max_n) return;
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failed */
    cache = new_cache;
    /* initialise new entries to zero */
    for (idx_t i = max_n + 1; i <= n; ++i)
        cache[i] = (seq_t)0;
    max_n = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(0) is always 1 */
    if (n == 0) return (seq_t)1;

    /* Ensure the cache is large enough and recompute from scratch
       whenever we see a larger n.  This guarantees correctness
       for arbitrary call order and prevents missing contributions
       that we cannot later recover.  */
    if (n > max_n)
    {
        /* Allocate or extend the array */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) abort();
        /* Zero newly added entries */
        for (idx_t i = max_n + 1; i <= n; ++i)
            cache[i] = (seq_t)0;
        max_n = n;

        /* Reset entire table for recomputation */
        for (idx_t i = 0; i <= n; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;          /* a(0) = 1 */

        /* Dynamic programming: multiply each factor (1 + m q^m)^21 */
        for (idx_t m = 1; m <= n; ++m)
        {
            /* Determine largest t such that t*m <= n, but t <= 21 */
            idx_t kmax = 21;
            if (m * kmax > n)
                kmax = n / m;
            if (kmax == 0)
                continue;            /* m is larger than n */

            /* Precompute coefficients bin[t] * m^t for t = 1 .. kmax */
            seq_t coeff[22];
            seq_t powmt = (seq_t)1;   /* m^0 */
            for (idx_t t = 1; t <= kmax; ++t)
            {
                powmt = powmt * (seq_t)m;       /* m^t */
                coeff[t] = bin[t] * powmt;      /* binomial * m^t */
            }

            /* Convolution: update cache from high indices downwards */
            for (idx_t s = n; ; --s)
            {
                seq_t val = cache[s];
                if (val != (seq_t)0)
                {
                    for (idx_t t = 1; t <= kmax; ++t)
                    {
                        idx_t idx = s + m * t;
                        if (idx > n) break;
                        cache[idx] = cache[idx] + (val * coeff[t]);
                    }
                }
                if (s == 0) break;
            }
        }
    }
    /* Cache already contains a(n) for all n <= max_n */
    return cache[n];
}
