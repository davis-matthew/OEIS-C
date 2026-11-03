/*
 * A192909  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) - a(n-5) Initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=9, a(4)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 994 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of previously computed terms */
    static idx_t max_computed = (idx_t)-1;   /* largest index currently stored */
    static size_t cap = 0;           /* allocated capacity of cache (elements) */

    /* make sure base terms are initialised */
    if (max_computed == (idx_t)-1) {
        /* need space for indices 0..4 */
        size_t need = 5;
        if (need > cap) {
            ensure_capacity(need, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        }
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 9;
        cache[4] = 27;
        max_computed = 4;
    }

    /* if requested term is already cached, return it immediately */
    if (n <= max_computed) return cache[n];

    /* extend the cache up to n */
    size_t need = (size_t)(n + 1);
    if (need > cap) {
        ensure_capacity(need, &cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Use mul_small to avoid overflow of unsigned integers.
           mul_small(x,m) returns x * m as a seq_t value. */
        seq_t val = mul_small(cache[i-1], 4);              /* 4*a(i-1)   */
        val = val - mul_small(cache[i-2], 3);              /* -3*a(i-2)   */
        val = val + cache[i-3];                            /* +a(i-3)     */
        val = val - cache[i-5];                            /* -a(i-5)     */
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
