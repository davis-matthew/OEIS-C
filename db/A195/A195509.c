/*
 * A195509  Expansion of e.g.f. (exp(x*exp(x)) + exp(x/exp(x)))/2.
 * https://oeis.org/A195509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..floor((n-1)/2)} binomial(n,2*m)*(n-2*m)^(2*m)
 *            a(0) = 1 Implements the sum directly while caching computed
 *            terms. Uses the provided big_pow and seq_one helper functions.
 *            The cache is zero‑initialized with calloc; 0 denotes "not yet
 *            computed"
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"
#include "../utils/seq_one.h"

/* -- multiplicative binomial coefficient C(n,k) -------------------------------- */
static seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return seq_one();          /* 0 */
    if (k > n - k) k = n - k;             /* symmetry */
    seq_t res = seq_one();
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i;   exact division holds */
        res = (res * (n - k + i)) / i;
    }
    return res;
}

/* -- cached sequence ------------------------------------------------------------ */
static seq_t *cache = NULL;      /* array of seq_t, zero means "not computed" */
static size_t cache_size = 0;    /* number of elements stored */

/* ------------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure cache large enough */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_cache) return seq_one();   /* fallback on OOM */

        /* copy old values */
        if (cache && cache_size > 0) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }

        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute all missing terms up to n */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (cache[i] != 0) continue;   /* already computed or a(0)=1 */

        seq_t val = seq_one();             /* m = 0 contributes 1 */
        if (i > 0) {
            idx_t maxm = (i - 1) / 2;
            for (idx_t m = 1; m <= maxm; ++m) {
                idx_t k   = 2 * m;
                seq_t bin = binomial_calc(i, k);
                seq_t base = (seq_t)(i - k);
                seq_t exp  = (seq_t)k;
                seq_t powv = big_pow(base, exp);
                val = val + (bin * powv);
            }
        }
        cache[i] = val;
    }

    return cache[(size_t)n];
}
