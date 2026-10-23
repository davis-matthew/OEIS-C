/*
 * A147678  Double, add 0, double, add 1, double, add 2, double, add 3, etc.
 * https://oeis.org/A147678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=2, a(n)=2*a(n-1) for even n, a(n)=a(n-1)+(n-3)/2 for odd n */
/* This implements a(n) for all n ≥ 1, using a cache for arbitrary order of calls. */
#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of sequence values, 1‑based index */
static size_t cache_cap = 0;         /* allocated capacity of the cache */
static size_t cache_len = 0;         /* number of terms currently stored (initially 0) */

/* initial terms a(1)..a(6) */
static const seq_t init_terms[6] = {
    (seq_t)1, (seq_t)2, (seq_t)2,
    (seq_t)4, (seq_t)5, (seq_t)10
};

/* ensure the cache can hold at least needed terms */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort program in a portable way */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) with memoization */
seq_t a(idx_t n)
{
    if (n <= 6)                     /* trivially stored terms */
        return init_terms[n - 1];

    if (cache_len == 0) {           /* first call – initialise cache with a(1)..a(6) */
        ensure_cache(6);
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_terms[i];
        cache_len = 6;
    }

    /* grow cache up to n */
    if (cache_len < n) {
        ensure_cache(n);
        while (cache_len < n) {
            idx_t idx = (idx_t)(cache_len + 1);   /* 1‑based index of the term to compute */
            seq_t prev = cache[cache_len - 1];
            seq_t val;
            if ((idx & 1) == 0) {   /* idx even: a(idx) = 2 * a(idx-1) */
                val = prev * (seq_t)2;
            } else {                /* idx odd: a(idx) = a(idx-1) + (idx-3)/2 */
                seq_t add = ((seq_t)(idx - 3)) / 2;
                val = prev + add;
            }
            cache[cache_len] = val;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
