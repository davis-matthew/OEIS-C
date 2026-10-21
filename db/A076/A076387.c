/*
 * A076387  Numbers n such that sum of digits in base 9 is a divisor of sum of
 *          prime divisors (A008472).
 * https://oeis.org/A076387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3461, u32 n <= 5189, u64 n <= 5189,
 *            u128 n <= 5189, big any n
 * Helpers:   digit_sum_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076387 – nth positive integer m whose sum of base‑9 digits divides
   the sum of its distinct prime divisors. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum_base9.h"

/* Returns the sum of distinct prime divisors of n.  The calculation is
   performed by trial division; the result is stored in seq_t. */
static seq_t sum_distinct_prime_divisors(uint64_t n)
{
    seq_t sum = 0;
    if (n < 2) return sum;   /* should not happen for n >= 2 */

    if ((n & 1) == 0) {
        sum += (seq_t)2;
        while ((n & 1) == 0) n >>= 1;
    }

    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            sum += (seq_t)p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) sum += (seq_t)n;   /* remaining prime factor */
    return sum;
}

/* Cache of the sequence terms.  cache[i] stores a(i+1). */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;   /* start searching from 2 */

/* Grow the cache to hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Main function requested by the exercise. */
seq_t a(idx_t n)
{
    /* If the cache already contains the n‑th term, return it. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Search for new terms until the cache contains at least n elements. */
    while (cache_len < (size_t)n) {
        uint64_t val = next_candidate++;

        seq_t digit_sum = digit_sum_base9(val);
        if (digit_sum == 0) continue;   /* should never happen for val >= 2 */

        seq_t prime_sum = sum_distinct_prime_divisors(val);
        if (prime_sum % digit_sum == 0) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)val;
        }
    }

    return cache[n - 1];
}
