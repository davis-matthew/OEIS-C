/*
 * A079599  Numbers n for which the n-th impartial game is a second player
 *          win.
 * https://oeis.org/A079599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; For n ≥ 0, a(n+1) is the smallest integer > a(n) whose
 *            binary representation has a 0 in every bit position a(0), a(1),
 *            …, a(n)
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 9996, u64 n <= 9996,
 *            u128 n <= 9996, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Cache for previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_size = 0;  /* number of terms stored */

/* Mask of forbidden bit positions that lie in the range [0,63].
   Only bits < 64 can influence values that fit in 64‑bit types;
   for arbitrary‑precision types the cast to seq_t handles
   the remaining bits automatically. */
static uint64_t small_mask = 0;

/* Compute a(n).  Results for all indices up to n are cached
   so that subsequent calls with larger or smaller n are efficient. */
seq_t a(idx_t n)
{
    /* Allocate or grow the cache if needed. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        small_mask = 0;
    }
    if ((size_t)n >= cache_size) {
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* Initialise the first term if the cache was empty. */
        if (cache_size == 0) {
            cache[0] = 0;
            cache_size = 1;
            small_mask = 1ULL << 0;   /* forbid bit 0 */
        }
        /* Compute missing terms up to n. */
        for (size_t idx = cache_size; idx <= (size_t)n; ++idx) {
            seq_t prev = cache[idx - 1];
            seq_t x = prev + 1;
            seq_t mask = (seq_t)small_mask;
            /* Find the first x that has no forbidden bits set. */
            while ((x & mask) != 0) x++;
            cache[idx] = x;
            /* Add the new term to the set of forbidden positions. */
            if (x < 64) {
                small_mask |= (1ULL << (unsigned)x);
            }
        }
        cache_size = n + 1;
    }
    return cache[(size_t)n];
}
