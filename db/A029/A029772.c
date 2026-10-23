/*
 * A029772  Every digit that appears in k also appears at least once in k^2.
 * https://oeis.org/A029772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029772: numbers k such that every digit of k appears in k^2 */
#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* generated terms */
static size_t cache_len = 0;         /* number of terms cached */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t last_checked = 0;       /* next candidate to examine */

/* Helper: add a candidate to the cache, grow if necessary */
static void add_candidate(seq_t k)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);   /* out of memory */
    }
    cache[cache_len++] = k;
}

/* Helper: compute digit mask of a seq_t number. It returns a 10‑bit mask
   where bit d is set if digit d occurs. */
static unsigned digit_mask(seq_t x)
{
    unsigned m = 0u;
    if (x == 0) return 1u << 0;        /* mask for digit 0 */
    while (x > 0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        m |= 1u << d;
        x /= (seq_t)10;
    }
    return m;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        seq_t k  = (seq_t)last_checked;
        seq_t sq = k * k;
        unsigned mask_k = digit_mask(k);
        unsigned mask_sq = digit_mask(sq);

        /* condition: every digit of k is present in k^2 */
        if ((mask_k & ~mask_sq) == 0u) {
            add_candidate(k);
        }
        last_checked++;
    }
}

/* Main function required by the building system. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1 (a(1) = 0).  n is guaranteed to be >= 1. */
    ensure_cache(n);
    return cache[n - 1];
}
