/*
 * A019506  Hoax numbers: composite numbers whose digit-sum equals the sum of
 *          the digit-sums of its distinct prime factors.
 * https://oeis.org/A019506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 2795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Hoax numbers: composite numbers whose digit-sum equals the sum of the digit-sums
 * of its distinct prime factors.  The function a(n) returns the nth such
 * number (offset 1).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "../utils/is_prime_small.h"
#include "../utils/digit_sum.h"
#include "../utils/_smallest_prime_factor.h"

/* Cache of found hoax numbers. */
static seq_t *cache = NULL;      /* array of seq_t hoax numbers */
static size_t cache_len = 0;     /* number of stored hoax numbers */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static uint64_t last_checked = 3;/* last integer examined (primes 2,3 excluded) */

/* Ensure the cache is allocated with a minimal capacity. */
static void init_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        assert(cache != NULL);
    }
}

/* Grow the cache when more space is required. */
static void grow_cache(void)
{
    size_t old_cap = cache_cap;
    cache_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *newp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    assert(newp != NULL);
    cache = newp;
}

/* Helper: sum of digit-sums of distinct prime factors of x (uint64_t). */
static unsigned int sum_distinct_prime_digit_sums(uint64_t x)
{
    unsigned int sum = 0;
    uint64_t n = x;
    uint64_t last_p = 0;

    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        if (p != last_p) {
            seq_t pd = digit_sum((seq_t)p);
            sum += (unsigned int)pd;
            last_p = p;
        }
        do {
            n /= p;
        } while (n % p == 0);
    }
    return sum;
}

/* Return the n‑th hoax number (offset 1). */
seq_t a(idx_t n)
{
    init_cache();

    /* Extend cache until we have at least n entries. */
    while (cache_len < (size_t)n) {
        uint64_t x = ++last_checked;

        /* Skip primes – hoax numbers are composite. */
        if (is_prime_small((seq_t)x))
            continue;

        /* Digit sum of the candidate number. */
        seq_t cand_digit_sum = digit_sum((seq_t)x);
        unsigned int cand_val = (unsigned int)cand_digit_sum;

        /* Sum the digit sums of the distinct prime factors. */
        unsigned int sum_fact = sum_distinct_prime_digit_sums(x);

        if (sum_fact == cand_val) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)x;
        }
    }

    /* Return the requested term (array is 0‑based). */
    return cache[n - 1];
}
