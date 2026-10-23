/*
 * A043751  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 5 mod 8.
 * https://oeis.org/A043751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   run_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: numbers n such that the number of runs in the binary
 * representation of n is congruent to 5 modulo 8.
 * Example:
 *   21 = 10101 (5 runs), 37 = 100101 (5 runs), …
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/run_count_u64.h"

static seq_t *cache = NULL;      /* cached values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  cache_len = 0;     /* number of values stored */
static idx_t  next_candidate = 0;/* next integer to test */

/* Grow the cache to hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least 'target' elements. */
static void ensure_cache(idx_t target)
{
    if (cache_len >= target) return;
    grow_cache(target);
    while (cache_len < target) {
        ++next_candidate;
        unsigned long long m = (unsigned long long)next_candidate;
        int runs = run_count_u64(m);
        if (runs % 8 == 5) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; behaviour below is undefined */
    ensure_cache(n);
    return cache[n - 1];
}
