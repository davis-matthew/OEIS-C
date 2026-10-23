/*
 * A127864  Number of tilings of a 2 X n board with 1 X 1 and L-shaped tiles
 *          (where the L-shaped tiles cover 3 squares).
 * https://oeis.org/A127864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=5; a(n)=a(n-1)+4*a(n-2)+2*a(n-3) for n≥3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache has at least 'need' elements (0‑based index). */
static void grow_cache(size_t need)
{
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= need) new_cap <<= 1;          /* double until sufficient */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                             /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence term a(n) */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;                        /* idx_t is unsigned 64‑bit */

    /* One‑time allocation of the cache on first call. */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
    }

    /* Expand cache storage if necessary. */
    if (idx >= cache_cap) grow_cache(idx);

    /* Compute missing terms up to idx. */
    while (cache_len <= idx) {
        seq_t val;
        if (cache_len == 0)           val = 1;          /* a(0) */
        else if (cache_len == 1)      val = 1;          /* a(1) */
        else if (cache_len == 2)      val = 5;          /* a(2) */
        else {
            seq_t a1 = cache[cache_len - 1];
            seq_t a2 = cache[cache_len - 2];
            seq_t a3 = cache[cache_len - 3];
            val = a1 + 4 * a2 + 2 * a3;             /* recurrence */
        }
        cache[cache_len++] = val;
    }
    return cache[idx];
}
