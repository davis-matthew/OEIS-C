/*
 * A054011  n is not divisible by the number of its proper divisors.
 * https://oeis.org/A054011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_proper_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054011: n is not divisible by the number of its proper divisors. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/count_proper_divisors.h"

/* Cache for previously computed terms of the sequence. */
static seq_t      *cache = NULL;
static size_t     cache_len = 0;   /* number of terms currently stored       */
static size_t     cache_cap = 0;   /* allocated capacity of cache             */
static uint64_t   next_candidate = 1;   /* next integer to test                 */

/* Ensure that the cache can store at least 'needed' terms. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    /* Grow the capacity by doubling until enough. */
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        abort(); /* Out of memory */
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Append a term to the cache. */
static void append_candidate(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Offset < 1; undefined value */

    while (cache_len < n) {
        uint64_t m = next_candidate;
        uint64_t prd = count_proper_divisors(m);
        if (prd > 1) {               /* at least one proper divisor */
            /* m is not divisible by its number of proper divisors */
            if (m % prd != 0) {
                append_candidate((seq_t)m);
            }
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
