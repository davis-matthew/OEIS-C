/*
 * A043665  Numbers whose base-14 representation has exactly 4 runs.
 * https://oeis.org/A043665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   count_base14_runs, get_cache_capacity, get_cache_size,
 *            set_cache, set_cache_capacity, set_cache_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by enumerating base-14 numbers that have exactly 4 runs. */
/* A(n) = the n-th positive integer whose base‑14 representation
   consists of exactly four maximal runs of equal digits. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_base14_runs.h"
#include "../utils/get_cache_capacity.h"
#include "../utils/get_cache_size.h"
#include "../utils/set_cache.h"
#include "../utils/set_cache_capacity.h"
#include "../utils/set_cache_size.h"

/* Grow the cache to at least 'need' elements. */
static void ensure_cache_size(size_t need)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    if (need <= capacity)
        return;

    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* in a real environment we would check for NULL, but this
       is sufficient for the short‑lived context of a sequence. */
    cache = new_cache;
    capacity = new_cap;
    /* store back via static local of the caller */
    *((seq_t **)(&cache)) = cache;
    *((size_t *)(&capacity)) = capacity;
}

/* This function maintains its own static cache. */
static seq_t *get_cache(void)
{
    static seq_t *cache = NULL;
    return cache;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;
    static seq_t current = 1;   /* next number to test */

    /* Grow cache if necessary. */
    while (cache_size < n) {
        /* Find next number with 4 runs. */
        while (count_base14_runs(current) != 4)
            ++current;
        /* Grow cache storage if full. */
        if (cache_size == cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_cache;
            cache_capacity = new_cap;
        }
        cache[cache_size++] = current;
        ++current;
    }

    return cache[n - 1];
}
