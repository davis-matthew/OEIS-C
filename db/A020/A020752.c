/*
 * A020752  Pisot sequence T(7,10), a(n) = floor(a(n-1)^2/a(n-2)).
 * https://oeis.org/A020752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)^2 / a(n-2)), a(0)=7, a(1)=10
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 107, u64 n <= 230,
 *            u128 n <= 225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static cache for computed terms */
static seq_t *cache = NULL;      /* array of cached values */
static size_t cache_len = 0;     /* number of values stored */

/* ------------------------------------------------------------------ */
/* Helper: compute one term from the previous two.                     */
/* ------------------------------------------------------------------ */
static inline seq_t compute_term(seq_t prev2, seq_t prev1)
{
#ifdef __cplusplus
    /* use class operators for arbitrary‑precision types */
    return (prev1 * prev1) / prev2;
#else
    /* use unsigned __int128 to avoid overflow for built‑in types */
    unsigned __int128 prod = (unsigned __int128)prev1 * prev1;
    prod /= prev2;
    return (seq_t)prod;
#endif
}

/* ------------------------------------------------------------------ */
/* Ensure that cache contains values up to index n (inclusive).       */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (cache_len >= target)
        return;   /* already computed */

    /* Allocate or resize the cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc(target * sizeof(seq_t));
        if (cache == NULL)
            abort();  /* out of memory */
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (tmp == NULL)
            abort();  /* out of memory */
        cache = tmp;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i < target; ++i) {
        if (i == 0)
            cache[i] = 7;
        else if (i == 1)
            cache[i] = 10;
        else
            cache[i] = compute_term(cache[i - 2], cache[i - 1]);
    }
    cache_len = target;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
