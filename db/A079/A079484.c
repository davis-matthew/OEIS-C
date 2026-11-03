/*
 * A079484  a(n) = (2n-1)!! * (2n+1)!!, where the double factorial is A006882.
 * https://oeis.org/A079484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^2 - 1) * a(n-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 209 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache */
    static size_t cache_len = 0;   /* number of stored terms */

    /* Base case: a(0) = 1 */
    if (n == 0) {
        if (cache == NULL) {
            /* initialise cache with a(0) */
            cache_cap = 1;
            cache = alloc_seq_array(cache_cap);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Initialise cache if not done yet */
    if (cache == NULL) {
        cache_cap = 1;
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Ensure the cache can hold terms up to n */
    if ((size_t)n + 1 > cache_cap) {
        grow_seq_array(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Compute missing terms using the recurrence */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        /* factor = 4*k*k - 1  (fits in a 32‑bit unsigned int for all n <= 10000) */
        uint64_t factor = 4ULL * k * k - 1ULL;
        /* Multiply previous term by factor using mul_small to keep arithmetic cheap */
        cache[k] = mul_small(cache[k - 1], (unsigned int)factor);
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
