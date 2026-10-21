/*
 * A063818  Growth series for fundamental group of orientable closed surface
 *          of genus 8.
 * https://oeis.org/A063818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=32, and for n≥2
   a(n) = 31*a(n-1) - 31*a(n-16) + a(n-17) + b(n),
   where b(0)=b(1)=1, b(16)=b(17)=-1, and b(n)=0 otherwise. */

/* The sequence has generating function (1+x)*(1-x^16)/(1-31x+31x^16-x^17). */

#include <stdlib.h>

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap   = 0; /* capacity in elements */
static size_t cache_size  = 0; /* number of elements filled */

/* Ensure the cache holds terms up to and including index n. */
static void ensure_cache(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* out‑of‑memory: abort */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_size; i < needed; ++i) {
        seq_t term = 0;

        if (i >= 1)          term += (seq_t)31 * cache[i - 1];
        if (i >= 16)         term -= (seq_t)31 * cache[i - 16];
        if (i >= 17)         term +=          cache[i - 17];

        /* Add the inhomogeneous part */
        if (i == 0 || i == 1)     term += 1;
        else if (i == 16 || i == 17) term -= 1;

        cache[i] = term;
    }
    cache_size = needed;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[(size_t)n];
}
