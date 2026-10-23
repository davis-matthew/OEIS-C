/*
 * A177447  G.f.: Sum_{n>=0} a(n)*x^n/(1+x)^(n^2) = 1+x.
 * https://oeis.org/A177447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A177447
 *  Formula: a(0)=1, a(n)=1 - Σ_{k=0}^{n-1} a(k)·(-1)^{n-k}·C(k*(k-1), n-k)   (n≥1)
 *  The sequence is computed lazily and memoised; any call order is supported.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_size = 0;     /* number of cached elements */

/* Return a(n) for the OEIS sequence A177447. */
seq_t a(idx_t n)
{
    /* initialise cache for a(0) */
    if (!cache) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
    }
    /* if already computed, just return */
    if (n < cache_size) return cache[n];

    /* extend cache to hold values up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                  /* allocation failure */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t m = cache_size; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t t = m - k;                            /* t = n-k   */
            seq_t bin = binom(k * (k - 1), t);          /* C(k^2-k, t) */
            if (bin == (seq_t)0) continue;             /* skip zero */
            seq_t prod = bin * cache[k];                /* a(k) * bin */
            if (t & 1) sum -= prod;                     /* (-1)^{t} */
            else      sum += prod;
        }
        cache[m] = (seq_t)1 - sum;                      /* a(m)   */
    }
    cache_size = new_len;
    return cache[n];
}
