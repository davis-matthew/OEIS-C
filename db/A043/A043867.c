/*
 * A043867  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 0 mod 10.
 * https://oeis.org/A043867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   runs_divisible_by_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043867:
   Numbers n whose base‑4 representation has a number of runs
   that is divisible by 10.  a(1) = 279620.
   The function looks for the n‑th such number by scanning all
   natural numbers in increasing order and keeping those that
   satisfy the condition.  Results are cached for fast subsequent
   queries. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/runs_divisible_by_10.h"

/* Cached list of numbers found so far. */
static seq_t *cache     = NULL;  /* array of seq_t values */
static size_t cache_len = 0;     /* how many values are stored */
static size_t cache_cap = 0;     /* current allocated capacity */
static idx_t   next_scan = 1;    /* next integer to examine */

/* Ensure that the cache can hold at least 'min_cap' entries. */
static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();  /* out of memory */
        cache      = new_cache;
        cache_cap  = new_cap;
    }
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 */

    /* Extend the cache until we have the n-th element. */
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_scan;
        if (runs_divisible_by_10(val)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_scan;
    }
    return cache[(size_t)n - 1];
}
