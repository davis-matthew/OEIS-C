/*
 * A165504  Primes p with a digits sum of p^2 equal to 43.
 * https://oeis.org/A165504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime p such that the decimal digit sum of p^2
 *            equals 43. Computed by on‑demand primality testing and scanning
 *            for the condition
 * Range:     u8 none, u16 n <= 753, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_square_is_43, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"      /* bool is_prime_small(uint64_t n) */
#include "../utils/digit_sum_square_is_43.h"

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* capacity of cache */
static size_t cache_len = 0;     /* number of values cached */

/* Append a new sequence value, reallocating if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1; behaviour for n<1 is irrelevant */

    /* Start scanning for primes on demand. */
    static uint64_t candidate = 2;    /* next number to test for primality */

    /* Scan until we have at least n cached values. */
    while (cache_len < (size_t)n)
    {
        /* Skip even numbers after 2. */
        if (candidate > 2 && candidate % 2 == 0)
        {
            candidate++;
            continue;
        }

        /* Check primality and the digit-sum condition. */
        if (is_prime_small(candidate))
        {
            if (digit_sum_square_is_43(candidate))
            {
                append_cache((seq_t)candidate);
            }
        }

        /* Move to the next candidate. */
        candidate++;
    }

    return cache[(size_t)n - 1];
}
