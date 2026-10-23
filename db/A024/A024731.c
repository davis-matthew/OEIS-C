/*
 * A024731  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=a(2)=1 and a(3)=6.
 * https://oeis.org/A024731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *            a(1)=a(2)=1 and a(3)=6
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* -----  Cache for computed values  ----- */
static seq_t *cache       = NULL;   /* array of a(i), 0..max_index   */
static idx_t  cache_max   = 0;      /* largest index already computed */
static size_t cache_cap   = 0;      /* current allocated capacity (elements) */

/* ----  Ensure indices up to 'needed' are computed  ---- */
static void _ensure_cache(idx_t needed)
{
    if (needed <= cache_max)         /* nothing to do */
        return;

    /* Resize the cache if required */
    size_t new_cap = (size_t)(needed + 1);   /* need indices 0..needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);   /* out of memory */
        cache_cap = new_cap;
        /* initialise the first few known terms */
        cache[0] = 0;                    /* unused, but keep zero */
        if (needed >= 1) cache[1] = 1;
        if (needed >= 2) cache[2] = 1;
        if (needed >= 3) cache[3] = 6;
        cache_max = (needed < 3) ? needed : 3;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to 'needed' */
    for (idx_t k = cache_max + 1; k <= needed; ++k) {
        if (k <= 3) {          /* already set */
            if (k == 1) cache[1] = 1;
            if (k == 2) cache[2] = 1;
            if (k == 3) cache[3] = 6;
            continue;
        }
        seq_t sum = 0;
        idx_t limit = (k + 1) / 4;          /* floor((k+1)/4) */
        for (idx_t i = 1; i <= limit; ++i) {
            idx_t left  = 2 * i - 1;
            idx_t right = k - 2 * i + 1;
            sum += cache[(size_t)left] * cache[(size_t)right];
        }
        cache[(size_t)k] = sum;
    }
    cache_max = needed;
}

/* ----  Public entry point  ---- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n=0; return 0 safely */
    _ensure_cache(n);
    return cache[(size_t)n];
}
