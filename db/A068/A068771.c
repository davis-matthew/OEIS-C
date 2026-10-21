/*
 * A068771  Generalized Catalan numbers 9*x*A(x)^2 -A(x) +1 -8*x=0.
 * https://oeis.org/A068771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A068771: Generalized Catalan numbers
 * Recurrence: a(0)=1, a(1)=1, for n>=1
 *   a(n+1) = 9 * sum_{k=0..n} a(k)*a(n-k)
 * (works for any integer type seq_t; uses 128‑bit intermediate when seq_t is built‑in)
 */

#include <stddef.h>
#include <stdint.h>
/* Note: seq_t, idx_t are supplied by the build; do NOT define them here. */

static void ensure_cache_size(size_t needed, seq_t **c, size_t *cap)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? *cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*c, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    *c     = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of elements already stored */

    /* Expand cache as necessary */
    ensure_cache_size((size_t)n + 2, &cache, &cache_cap);   /* +2 for a(n+1) if needed */

    /* Initialise first two terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Compute missing terms up to n (and a[n+1] for recurrence) */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 1) continue;  /* a(0) and a(1) already set */

#ifdef __cplusplus
        /* C++ build: seq_t may be an arbitrary‑precision class. */
        seq_t sum = 0;
        for (size_t k = 0; k <= i-1; ++k)
            sum += cache[k] * cache[i-1-k];
        cache[i] = sum * (seq_t)9;
#else
        /* C build: seq_t is a built‑in unsigned integer type. */
        __uint128_t sum = 0;
        for (size_t k = 0; k <= i-1; ++k) {
            __uint128_t term = (__uint128_t)cache[k] * (__uint128_t)cache[i-1-k];
            sum += term;
        }
        cache[i] = (seq_t)(sum * 9);
#endif
    }

    cache_len = (n < cache_len) ? cache_len : (size_t)n + 1;  /* keep length updated */
    return cache[n];
}
