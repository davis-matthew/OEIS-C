/*
 * A132982  The non-prime-power "antimutinous" numbers. (Antimutinous numbers
 *          are those integers m, m >1, where m/p^k < p, p = largest prime
 *          divisor of m, p^k = largest power of p that divides m.)
 * https://oeis.org/A132982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth non‑prime‑power antimutinous number (antimutinous: m
 *            / p^k < p, with p the largest prime divisor, p^k the highest
 *            power of p dividing m). This implementation enumerates integers
 *            and stores them in a static cache, so that calls with arbitrary
 *            n are answered efficiently. It works for any...
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_antimutinous_np
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_antimutinous_np.h"

/* Cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */
static uint64_t next_candidate = 2;   /* next integer to test */

/* Ensure cache contains at least n terms. */
static void ensure_cache_up_to(idx_t n)
{
    if (cache_size >= n)
        return;

    /* Extend cache capacity as needed. */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_size < n) {
        while (!is_antimutinous_np(next_candidate))
            ++next_candidate;

        /* store current candidate */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* Main sequence function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1; a(0) irrelevant, return 0 */
        return (seq_t)0;
    ensure_cache_up_to(n);
    return cache[n - 1];
}
