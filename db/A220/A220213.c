/*
 * A220213  Majority value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their king-move neighbors in a random 0..1 n X 2 array.
 * https://oeis.org/A220213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 3*a(n-3) + 7*a(n-4) + 9*a(n-5) * +
 *            12*a(n-6) + 11*a(n-7) + 4*a(n-8) + 4*a(n-9) * Initial values:
 *            a(1)=2, a(2)=6, a(3)=23, a(4)=75, a(5)=241, * a(6)=777,
 *            a(7)=2512, a(8)=8132, a(9)=26335. * * This implementation
 *            records the sequence in a static cache that is * lazily g...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc, size_t */
#include <string.h>   /* memcpy */

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache array */
static idx_t cached_max = 0;   /* largest index computed so far (>=0) */

/* Grow the cache so that it can store indices up to 'needed' (inclusive). */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    if (cache) {
#ifdef __cplusplus
        for (size_t k = 0; k <= cached_max; ++k) new_cache[k] = cache[k];
        delete[] cache;
#else
        memcpy(new_cache, cache, (cached_max + 1) * sizeof(seq_t));
        free(cache);
#endif
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* out of offset, undefined but harmless */

    /* Convert to size_t for indexing; the conversion is safe because n>=1. */
    size_t idx = (size_t)n;

    /* If the value has already been computed, return it. */
    if (cache && cached_max >= n)
        return cache[idx];

    /* Initialise the cache with the first 9 terms if it is empty. */
    if (cache == NULL) {
        /* Allocate space for at least indices 0..9. */
        size_t init_cap = 10;
        ensure_capacity(init_cap);

        /* Base values: indices 1..9 (index 0 unused). */
        cache[1] = 2;
        cache[2] = 6;
        cache[3] = 23;
        cache[4] = 75;
        cache[5] = 241;
        cache[6] = 777;
        cache[7] = 2512;
        cache[8] = 8132;
        cache[9] = 26335;
        cached_max = 9;
    }

    /* Grow if required. */
    if (cache_cap <= idx)
        ensure_capacity(idx + 1);

    /* Compute missing values up to n. */
    size_t start = cached_max + 1;
    if (start < 10)   /* ensure all needed previous indices exist */
        start = 10;

    for (size_t i = start; i <= idx; ++i) {
        seq_t val = 0;
        val += 2 * cache[i - 1];
        val += 2 * cache[i - 2];
        val += 3 * cache[i - 3];
        val += 7 * cache[i - 4];
        val += 9 * cache[i - 5];
        val += 12 * cache[i - 6];
        val += 11 * cache[i - 7];
        val += 4 * cache[i - 8];
        val += 4 * cache[i - 9];
        cache[i] = val;
    }
    cached_max = n;

    return cache[idx];
}
