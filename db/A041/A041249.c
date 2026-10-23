/*
 * A041249  Denominators of continued fraction convergents to sqrt(136).
 * https://oeis.org/A041249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 70*a(n-4) - a(n-8) for n ≥ 8 Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=68, a(5)=71, a(6)=139, a(7)=210
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */

#include <stdlib.h>

/* static table of the first eight terms */
static const seq_t init[8] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
    (seq_t)68, (seq_t)71, (seq_t)139, (seq_t)210
};

/* cache of computed terms, allocated as needed */
static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of terms for which cache has been computed */

/* ensure that cache has space for at least 'needed' terms */
static inline void expand_cache(size_t needed)
{
    if (cache_size >= needed)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed: abort */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
    /* cache_size is not changed here – it reflects how many terms have
       actually been initialized, which will be updated after the loop
       below. */
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* work with a native index type */
    size_t idx = (size_t)n;

    /* make sure the cache is large enough */
    expand_cache(idx + 1);

    /* compute any missing values up to idx (in case calls are arbitrary‑ordered) */
    for (size_t i = cache_size; i <= idx; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            /* recurrence: a(i) = 70*a(i-4) - a(i-8) */
            cache[i] = ((seq_t)70 * cache[i - 4]) - cache[i - 8];
        }
    }

    /* update the number of cached values */
    cache_size = idx + 1;

    return cache[idx];
}
