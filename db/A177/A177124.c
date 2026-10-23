/*
 * A177124  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=8, k=1 and l=1.
 * https://oeis.org/A177124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A177124:
   a(0) = 1,
   a(1) = 8,
   for n >= 2: a(n) = Σ_{p=0}^{n-1} a(p) * a(n-1-p) + (n + 1). */

static seq_t *cache     = NULL;   /* cached sequence values */
static size_t cache_cap = 0;      /* allocated capacity of cache    */
static idx_t  cache_len = 0;      /* number of terms already computed */

static void ensure_cache(idx_t upto)
{
    /* Allocate or grow the cache to hold terms up to 'upto'. */
    if (cache_len == 0) {
        cache_cap = 16;                         /* initial capacity */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit((int)EXIT_FAILURE);
        /* initialise the known first two terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    if (cache_len > upto) return;               /* already have them */

    /* Grow capacity if necessary. */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)upto) new_cap <<= 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit((int)EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms iteratively. */
    for (idx_t i = cache_len; i <= upto; ++i) {
        if (i == 0) continue;  /* already set */
        if (i == 1) continue;  /* already set */

        /* a(i) = Σ_{p=0}^{i-1} a(p) * a(i-1-p) + (i + 1) */
        seq_t sum = (seq_t)0;
        for (idx_t p = 0; p < i; ++p) {
            sum += cache[(size_t)p] * cache[(size_t)(i-1-p)];
        }
        sum += (seq_t)(i + 1);   /* add (n + 1) */
        cache[(size_t)i] = sum;
    }

    cache_len = upto + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
