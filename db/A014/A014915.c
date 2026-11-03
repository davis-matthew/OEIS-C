/*
 * A014915  a(1)=1, a(n) = n*3^(n-1) + a(n-1).
 * https://oeis.org/A014915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*3 + 3*3^2 + … + n*3^(n-1) Recurrence: a(1)=1,
 *            a(n)=a(n-1)+n*3^(n-1) (n≥2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 575 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* Handle n=0 gracefully – the sequence is defined from n=1. */
    if (n == 0) return (seq_t)0;

    /* Static cache to allow random access order. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* largest n cached (≥1) */
    static idx_t cache_cap = 0;      /* capacity of cache array */
    static seq_t last_pow3 = 0;      /* 3^(cache_len-1) */

    /* Allocate and initialise cache on first call. */
    if (cache == NULL) {
        ensure_capacity(2, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        if (cache_cap == 0) {   /* safety check */
            cache_cap = 2;
            cache = mem_new(seq_t, cache_cap);
        }
        cache[1] = (seq_t)1;        /* a(1) = 1 */
        cache_len = 1;
        last_pow3 = (seq_t)1;       /* 3^(1-1) = 1 */
    }

    /* Return cached value if already computed. */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Expand cache to accommodate up to n. */
    ensure_capacity(n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    if (cache_cap < n + 1) {
        cache_cap = n + 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }

    /* Compute missing terms up to n. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Update last_pow3 to 3^(k-1). */
        last_pow3 = mul_small(last_pow3, 3);

        /* term = k * 3^(k-1).  k fits in unsigned int for the intended limits. */
        seq_t term = mul_small(last_pow3, (unsigned int)k);

        /* a(k) = a(k-1) + term. */
        cache[k] = cache[k - 1] + term;
    }

    cache_len = n;
    return cache[n];
}
