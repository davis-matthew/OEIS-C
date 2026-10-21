/*
 * A060557  Row sums of triangle A060556.
 * https://oeis.org/A060557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n‑1) – 6*a(n‑2) + a(n‑3), a(0)=1, a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* cached values of the sequence */
static seq_t *cache      = NULL;   /* pointer to the first term */
static idx_t  cache_len  = 0;      /* number of computed terms          */
static idx_t  cache_cap  = 0;      /* allocated capacity (in terms)     */

/* ensure the cache holds at least 'need' terms (index 0 … need‑1) */
static void ensure_capacity(idx_t need)
{
    if (cache_cap >= need)
        return;

    /* grow: double the capacity or set to need, whichever is larger */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_cache) /* allocation failed – just return and let caller fail */
        return;

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of sequence A060557 */
seq_t a(idx_t n)
{
    /* initialize cache on first call */
    if (cache_len == 0) {
        /* minimal capacity for first three terms */
        cache_cap = 3;
        cache     = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure → zero */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        cache_len = 3;
    }

    /* make sure cache can hold required term */
    if (n >= cache_cap) {
        ensure_capacity((idx_t)((n + 1) > cache_cap ? (n + 1) : cache_cap));
        if (cache_len < n + 1) {
            /* compute missing terms */
            for (idx_t i = cache_len; i <= n; ++i) {
                seq_t term = 5 * cache[i - 1];
                term -= 6 * cache[i - 2];
                term += cache[i - 3];
                cache[i] = term;
            }
            cache_len = n + 1;
        }
    } else if (n >= cache_len) {
        /* cache may be large enough but we still need to compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = 5 * cache[i - 1];
            term -= 6 * cache[i - 2];
            term += cache[i - 3];
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
