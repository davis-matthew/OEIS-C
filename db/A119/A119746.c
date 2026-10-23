/*
 * A119746  Sum of previous term and preceding relatively prime terms.
 * https://oeis.org/A119746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2 for n ≥ 2: a(n+1) = a(n) + Σ{ a(k) : 1 ≤ k <
 *            n and gcd(a(n), a(k)) = 1 }
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc */
#include "../utils/gcd.h"

/* Static cache for previously computed terms.  The array is 1‑based;
   index 0 is unused. */
static seq_t  *cache   = NULL;   /* pointer to allocated array */
static idx_t   cached_len = 0;   /* highest index that has been computed (≥0) */
static size_t  cache_cap  = 0;   /* number of elements currently allocated */

/* Ensure the cache has space to hold terms up to and including index n. */
static void
ensure_cache(idx_t n)
{
    size_t need_cap = (size_t)(n + 1);   /* +1 to ignore index 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(need_cap * sizeof(seq_t));
        cache_cap = need_cap;
    } else if (cache_cap <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, need_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = need_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* Make sure we have space for index n */
    ensure_cache(n);

    /* Initialise the first two terms if they haven't been set yet */
    if (cached_len < 1) {
        cache[1] = (seq_t)1;
        cached_len = 1;
    }
    if (cached_len < 2 && n >= 2) {
        cache[2] = (seq_t)2;
        cached_len = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t sum   = 0;

        for (idx_t k = 1; k < i; ++k) {
            if (gcd(prev, cache[k]) == (seq_t)1)
                sum += cache[k];
        }

        cache[i] = prev + sum;
    }

    cached_len = n;
    return cache[n];
}
