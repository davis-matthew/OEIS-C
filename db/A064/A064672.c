/*
 * A064672  a(0) = 0, a(1) = 1; for a(n), n >= 2, write n = x^2 + y with y >=
 *          0 as small as possible, then a(n) = a(x) + a(y).
 * https://oeis.org/A064672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = a(⌊√n⌋) + a(n - ⌊√n⌋²)
    a(0) = 0,  a(1) = 1
    (temperature of the definition in OEIS A064672)  */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------
 *  integer square root for a 64‑bit unsigned integer.
 *  Finds ⌊√n⌋ without using floating point.
 * -------------------------------------------------------------------- */
static inline idx_t floor_sqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t low = 0, high = (idx_t)1 << 32; /* sqrt(2¹⁶⁴−1) < 2³² */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (mid <= n / mid)      /* mid*mid <= n  */
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* --------------------------------------------------------------------
 *  dynamic cache for computed values of a(n).
 * -------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;

static void ensure_cache_idx(idx_t n) {
    if (n == 0) return;  /* nothing to allocate */
    if (cache == NULL) {
        /* capacity at least n+1, else some power of two */
        size_t capa = 16;
        while (capa <= (size_t)n) capa <<= 1;
        cache = (seq_t*)calloc(capa, sizeof(seq_t));
        if (cache == NULL) abort();   /* out of memory */
        cache_cap = capa;
        cache[0] = 0;
        cache[1] = 1;
        return;
    }
    if (n + 1 <= cache_cap) return;  /* already large enough */

    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) abort();  /* out of memory */
    /* Zero the newly allocated part */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
    cache[0] = 0;
    if (cache_cap > 1) cache[1] = 1;
}

/* --------------------------------------------------------------------
 *  Main recursive function with memoisation.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* base case, no caching needed */
    if (n == 1) return 1;          /* base case */

    ensure_cache_idx(n);
    if (cache[n] != 0) return cache[n];

    idx_t x = floor_sqrt_u64(n);
    idx_t y = n - x * x;
    seq_t val = a(x) + a(y);
    cache[n] = val;
    return val;
}
