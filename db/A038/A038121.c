/*
 * A038121  E.g.f.: (1 + 15*x + (45/2)*x^2 + (5/2)*x^3)/(1 - 2*x)^(13/2).
 * https://oeis.org/A038121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+3)*(2*n+5)/n) * a(n-1) , a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored: indices 0 .. cache_len-1 */
static size_t cache_cap = 0;   /* allocated slots */

/* Ensure cache can hold indices 0 .. n */
static void grow_cache_to(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) from a(n-1) */
static inline seq_t compute_next(idx_t i, seq_t prev)
{
#ifdef __SIZEOF_INT128__
#if !defined(__cplusplus)
    /* Use 128‑bit intermediate to avoid overflow (C only) */
    unsigned __int128 t = (unsigned __int128)prev;
    t *= (i + 3);
    t *= (2 * i + 5);
    t /= i;
    return (seq_t)t;
#else
    /* C++ build: we assume seq_t is a big‑int class */
    return prev * (seq_t)(i + 3) * (seq_t)(2 * i + 5) / (seq_t)i;
#endif
#else
    /* No 128‑bit support (C only): primitives will overflow after small n,
       but for such builds the sequence fits in seq_t only for very small n,
       so this branch is sufficient.  C++ big‑int builds use the fallback above. */
    return prev * (seq_t)(i + 3) * (seq_t)(2 * i + 5) / (seq_t)i;
#endif
}

seq_t a(idx_t n)
{
    if (cache_len == 0) {
        grow_cache_to(0);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    if ((idx_t)cache_len <= n) {
        grow_cache_to(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = compute_next(i, cache[i - 1]);
        }
        cache_len = (size_t)n + 1;
    }
    return cache[n];
}
