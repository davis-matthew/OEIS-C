/*
 * A133558  a(n) = a(n-1) + 9*a(n-2) for n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A133558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 9*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 394 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialisation of the cache */
    if (cache == NULL) {
        cache = seq_alloc(2);
        cache[0] = 1;
        cache[1] = 2;
        cache_len = 2;
        cache_cap = 2;
    }

    /* If requested value is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + (cache[i-2] * (seq_t)9);
    }
    cache_len = n + 1;

    return cache[n];
}
