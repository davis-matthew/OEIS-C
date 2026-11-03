/*
 * A048990  Catalan numbers with even index (A000108(2*n), n >= 0): a(n) =
 *          binomial(4*n, 2*n)/(2*n+1).
 * https://oeis.org/A048990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, 2*n)/(2*n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

#ifdef __cplusplus
#include <type_traits>
#endif

static seq_t compute_next(seq_t prev, idx_t i)
{
#ifdef __cplusplus
    /* C++ mode ------------------------------------------------------- */
    if constexpr (std::is_integral<seq_t>::value) {
        /* seq_t is an integer type (uint8_t … uint64_t) */
        unsigned __int128 num = (unsigned __int128)prev;
        num *= 2;
        num *= (unsigned __int128)(4*i-1);
        num *= (unsigned __int128)(4*i-3);
        unsigned __int128 denom = (unsigned __int128)i * (unsigned __int128)(2*i+1);
        return (seq_t)(num / denom);
    } else {
        /* seq_t is an arbitrary‑precision class */
        seq_t num = prev;
        num *= (seq_t)2;
        num *= (seq_t)(4*i-1);
        num *= (seq_t)(4*i-3);
        seq_t denom = (seq_t)i * (seq_t)(2*i+1);
        return num / denom;
    }
#else
    /* C mode ---------------------------------------------------------- */
    unsigned __int128 num = (unsigned __int128)prev;
    num *= 2;
    num *= (unsigned __int128)(4*i-1);
    num *= (unsigned __int128)(4*i-3);
    unsigned __int128 denom = (unsigned __int128)i * (unsigned __int128)(2*i+1);
    return (seq_t)(num / denom);
#endif
}

seq_t a(idx_t n)
{
    /* Static cache suitable for any build */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  last_computed = 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        ensure_cache(1, &cache, &cache_cap);      /* space for a(0) */
        cache[0] = (seq_t)1;                      /* a(0) = 1 */
        last_computed = 0;
    }

    if (n <= last_computed)
        return cache[n];

    /* Grow cache if necessary */
    ensure_cache(n + 1, &cache, &cache_cap);

    seq_t prev = cache[last_computed];
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = compute_next(prev, i);
        cache[i] = val;
        prev = val;
    }
    last_computed = n;
    return cache[n];
}
