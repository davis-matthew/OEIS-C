/*
 * A119613  Numbers n such that the difference between the largest distinct
 *          prime divisor and the smallest distinct prime divisor is a prime.
 * https://oeis.org/A119613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119613: numbers whose difference between the largest
   and smallest distinct prime divisors is itself a prime.
   Computed by brute‑force search, caching results. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime test supplied by the build environment. */
#include "../utils/is_prime_small.h"

/* Utility: factor n into its distinct prime divisors, returning the
   smallest and largest such factors.  Returns true if at least two
   distinct prime factors exist; otherwise false. */
static bool factor_min_max(idx_t n, idx_t *min, idx_t *max)
{
    idx_t tmp = n;
    idx_t min_val = 0, max_val = 0;

    /* Handle factor 2 separately. */
    if ((tmp & 1) == 0) {
        min_val = 2;
        max_val = 2;
        while ((tmp & 1) == 0)
            tmp >>= 1;
    }

    /* Test odd divisors. */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            if (min_val == 0)
                min_val = p;
            max_val = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* Remaining prime > sqrt(n). */
    if (tmp > 1) {
        if (min_val == 0)
            min_val = tmp;
        if (tmp > max_val)
            max_val = tmp;
    }

    if (min_val == 0 || min_val == max_val)
        return false;                /* only one distinct prime divisor */
    *min = min_val;
    *max = max_val;
    return true;
}

/* Cache for already found terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Remember the next integer to test.  Start at the first known term. */
static idx_t next_candidate = 10;

/* A(n): return the n-th term of sequence A119613. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined, but harmless */

    /* Produce terms until we have enough. */
    while (cache_size < (size_t)n) {
        /* Skip primes – they cannot satisfy the property. */
        if (!is_prime_small((seq_t)next_candidate)) {
            idx_t min, max;
            if (factor_min_max(next_candidate, &min, &max)) {
                idx_t diff = max - min;
                if (is_prime_small((seq_t)diff)) {
                    /* Append to cache. */
                    if (cache_size == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 8;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        if (!cache) abort();
                    }
                    cache[cache_size++] = (seq_t)next_candidate;
                }
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
