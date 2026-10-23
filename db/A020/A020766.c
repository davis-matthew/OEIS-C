/*
 * A020766  Expansion of g.f. 1/((1-6*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A020766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A020766: Expansion of g.f. 1/((1-6*x)*(1-11*x)*(1-12*x)).
 * Recurrence: a(0)=1, a(1)=29, 
 *             a(n) = 23*a(n-1) - 132*a(n-2) + 6^n   (n>=2)
 *
 * Implementation uses memoisation to allow calls with indices in any order.
 * All arithmetic is performed with type seq_t so that the result is exact 
 * whenever it fits in that type (or arbitrary‑precision when available).
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised a(i) values */
static size_t cache_size = 0;       /* currently allocated size of cache */
static idx_t  max_cached = (idx_t)-1;  /* largest n for which cache[n] is valid */
static seq_t  pow6_cur = (seq_t)1;   /* 6^(max_cached) */

/* Ensure the cache contains values up to index n */
static void
ensure_cache(idx_t n)
{
    if (!cache) {
        /* initialise cache for n = 0 and n = 1 */
        cache_size = 2;
        cache      = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        max_cached = 1;
        pow6_cur   = (seq_t)6;        /* 6^1 */
    }

    while (max_cached < n) {
        /* grow cache if necessary */
        if ((size_t)(max_cached + 1) >= cache_size) {
            size_t new_size = cache_size ? cache_size * 2 : 2;
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            cache_size = new_size;
        }

        /* compute next power of 6 */
        seq_t pow_val = pow6_cur * (seq_t)6;
        pow6_cur     = pow_val;

        /* recurrence: a(i) = 23*a(i-1) - 132*a(i-2) + 6^i */
        seq_t term = (cache[max_cached] * (seq_t)23)
                   - (cache[max_cached - 1] * (seq_t)132)
                   + pow_val;
        cache[++max_cached] = term;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
