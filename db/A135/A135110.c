/*
 * A135110  Positive numbers such that the digital sum base 2 and the digital
 *          sum base 10 are in a ratio of 2:10.
 * https://oeis.org/A135110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135110: a(n) is the nth positive integer x for which
   popcount(x)*5 == digit_sum(x) (the ratio of binary to decimal digit
   sums equals 2:10). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;   /* cache of found terms          */
static size_t cache_len = 0;  /* number of terms stored       */
static size_t cache_cap = 0;  /* current allocation capacity */
static seq_t next_candidate = 1;  /* next integer to test       */

/* Helper: test whether a number satisfies the condition. */
static bool is_valid(seq_t val)
{
    seq_t pop = 0;
    seq_t t   = val;
    while (t > 0) {
        pop += (t & 1);
        t >>= 1;
    }
    seq_t dec = digit_sum(val);
    return (pop * 5) == dec;
}

/* Helper: enlarge the cache when full. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failed – abort silently */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0, return 0 as fallback */

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        if (is_valid(next_candidate)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = next_candidate;
        }
        /* Increment candidate, careful with overflow on seq_t. */
        next_candidate += 1;
    }
    return cache[n - 1];
}
