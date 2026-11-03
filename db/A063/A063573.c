/*
 * A063573  First differences of A002065.
 * https://oeis.org/A063573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + 2*a(n-1)*sqrt(a(n-1)-1) + a(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 11 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Cached values of the sequence a(k), k = 0 .. cache_len-1 */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* current allocation size */
static idx_t  cache_len = 0;  /* number of terms currently stored */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Lazy initialization of cache. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;          /* a(0) */
        cache_len = 1;
    }

    /* Grow cache until it contains term a(n) */
    while (cache_len <= n) {
        /* Expand storage if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next term a(k) from a(k-1) */
        seq_t prev = cache[cache_len - 1];               /* a(k-1) */
        seq_t sqrt_term = floor_sqrt(prev - (seq_t)1);   /* sqrt(a(k-1)-1) */
        seq_t val = prev * prev
                  + ((seq_t)2 * prev * sqrt_term)
                  + prev;                                /* a(k) */
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
