/*
 * A041527  Denominators of continued fraction convergents to sqrt(280).
 * https://oeis.org/A041527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 502*a(n-6) - a(n-12)
   with initial values
   a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=11, a(5)=15,
   a(6)=491, a(7)=506, a(8)=1503, a(9)=2009, a(10)=5521, a(11)=7530 */
#include <stdlib.h>

#ifndef SEQ_T_DEFINED
/* The build system provides typedefs: seq_t and idx_t */
#endif

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity in elements */
static size_t cache_size = 0;  /* number of elements currently stored */

/* Pre‑computed first 12 terms. */
static const seq_t init_vals[12] = {
    (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)4,
    (seq_t)11, (seq_t)15, (seq_t)491, (seq_t)506,
    (seq_t)1503, (seq_t)2009, (seq_t)5521, (seq_t)7530
};

/* Ensure cache has capacity for indices 0..n (but don't change cache_size). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;            /* we need at least n+1 slots */
    if (cache == NULL) {
        /* Start with at least 12 elements (size of init_vals) */
        cache_cap = need > 12 ? need : 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();         /* allocation failed */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_size = 12;
    } else if (need > cache_cap) {
        /* Grow capacity (double until enough) */
        while (cache_cap < need) cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* reallocation failed */
        cache = tmp;
        /* new elements are uninitialized; they will be filled later */
    }
}

/* Return a(n). */
seq_t a(idx_t n)
{
    if (n < 12) return init_vals[(size_t)n];

    ensure_cache(n);

    /* Compute any missing terms from cache_size up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)502 * cache[i - 6] - cache[i - 12];
    }
    cache_size = (size_t)n + 1;   /* now all 0..n are initialized */

    return cache[(size_t)n];
}
