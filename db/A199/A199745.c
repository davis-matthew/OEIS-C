/*
 * A199745  Numbers such that the sum of the largest and the smallest prime
 *          divisor equals the sum of the other distinct prime divisors.
 * https://oeis.org/A199745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence of integers such that the sum of the largest and the
 *            smallest * distinct prime divisor equals the sum of the
 *            remaining distinct prime divisors. * This implementation finds
 *            terms sequentially, memoises them, and * supports random access
 *            to a(n) for any n >= 1
 * Range:     u8 none, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Cache of found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* Next integer to test. */
static uint64_t next_candidate = 2;

/* Append a newly found term to the cache, growing the array if necessary. */
static void append_value(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* Undefined for n < 1; return 0. */

    /* Generate terms until we have at least n values. */
    while (cache_len < (size_t)n) {
        uint64_t x = next_candidate++;

        /* Factorise x into distinct primes. */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t cnt;
        factor_u64(x, pr, ex, &cnt);

        if (cnt < 4)          /* Need at least four distinct primes. */
            continue;

        /* Compute sums of primes. */
        uint64_t sum = 0;
        for (size_t i = 0; i < cnt; ++i)
            sum += pr[i];

        uint64_t smallest = pr[0];
        uint64_t largest  = pr[cnt - 1];
        uint64_t others_sum = sum - smallest - largest;

        if (smallest + largest == others_sum)
            append_value((seq_t)x);
    }

    return cache[n - 1];
}
