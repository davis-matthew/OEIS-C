/*
 * A074364  Semi-tribonacci numbers, from tribonacci by analogy with
 *          semi-Fibonacci sequence.
 * https://oeis.org/A074364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 154, u32 n <= 1110, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   a(0) = 0
 *   a(1) = 1
 *   a(2n)   = a(n)
 *   a(2n+1) = a(2n)+a(2n-1)+a(2n-2)
 *   
 * This implementation memoises values in a static array so that multiple
 * calls in any order are O(1) after the first computation for a given
 * index.  It runs in O(n) time for the largest n requested.
 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_length = 0;

/* Ensure the cache has space for indices up to 'need' (inclusive). */
static void ensure_cache_capacity(idx_t need)
{
    size_t idx = (size_t)need;
    if (idx < cache_length) return;

    /* Grow capacity exponentially to maintain amortised O(1). */
    size_t new_cap = cache_capacity ? cache_capacity : 4;
    while (idx >= new_cap) new_cap <<= 1;

    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem) return; /* allocation failure – leave cache unchanged; */
    cache = new_mem;
    cache_capacity = new_cap;
}

/* Initialise cache with the first two terms. */
static void init_cache(void)
{
    if (!cache) {
        cache_capacity = 4;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_length = 2;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 1.  Initialise cache on first use. */
    init_cache();
    if (!cache) { /* allocation failure – return 0 as a fallback */
        return (seq_t)0;
    }

    /* 2.  Extend cache if needed and fill missing terms. */
    ensure_cache_capacity(n);
    if (!cache) return (seq_t)0; /* re-allocation failure fallback */

    for (size_t i = cache_length; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if ((i & 1) == 0) {           /* even index */
            cache[i] = cache[i / 2];
        } else {                              /* odd index */
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
    }
    cache_length = (size_t)n + 1;
    return cache[(size_t)n];
}
