/*
 * A145181  Continued cotangent recurrence a(n+1)=a(n)^3+3*a(n) and a(1)=7.
 * https://oeis.org/A145181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^3 + 3*a(n), a(1) = 7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <cstddef>
#endif

/* Cache for memoised terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least `need` terms */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;

    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
#endif

    /* copy old terms */
    for (size_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }

    /* free old storage */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Public function returning a(n) for n >= 1 (no defined behaviour for n==0) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, undefined */

    size_t idx = (size_t)n;               /* safe conversion */

    if (idx > cache_len) {
        ensure_capacity(idx);

        /* initialise first term if cache was empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)7;          /* a(1) = 7 */
            cache_len = 1;
        }

        /* compute terms up to required index */
        for (size_t i = cache_len; i < idx; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = (prev * prev * prev) + ((seq_t)3 * prev);
        }
        cache_len = idx;
    }

    return cache[idx - 1];                /* return a(n) */
}
