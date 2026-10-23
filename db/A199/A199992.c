/*
 * A199992  Composite numbers whose multiplicative persistence is 2.
 * https://oeis.org/A199992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199992: Composite numbers whose multiplicative persistence is 2.
   For each n ≥ 1, a(n) is the n‑th composite integer with persistence exactly 2. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions provided by the build environment. */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/digit_product.h" /* seq_t digit_product(seq_t n); */

/* True iff x > 1 and not prime, i.e. x is composite. */
static bool is_composite(seq_t x)
{
    if (x <= 3) {
        return false;                 /* 1, 2, 3 are not composite */
    }
    /* For larger numbers, rely on the prime test. */
    return !is_prime_big(x);
}

/* True iff the multiplicative persistence of x is exactly 2. */
static bool has_persistence_two(seq_t x)
{
    seq_t p1 = digit_product(x);
    if (p1 < 10) {
        return false;                 /* persistence 1 */
    }
    seq_t p2 = digit_product(p1);
    return p2 < 10;                   /* persistence 2 */
}

/* Main function: returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t  cached_len = 0;
    static size_t  cached_cap = 0;

    /* Current integer to test (type wide enough to hold arbitrary candidates). */
    static uint64_t current = 2;

    /* Initialise cache on first call. */
    if (cached_len == 0) {
        cached_cap = 64;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n terms. */
    while (cached_len < (size_t)n) {
        seq_t candidate = (seq_t)current;   /* value to store in cache */

        if (is_composite(candidate) && has_persistence_two(candidate)) {
            /* Grow cache if necessary. */
            if (cached_len == cached_cap) {
                cached_cap <<= 1;
                cache = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
            }
            cache[cached_len++] = candidate;
        }

        ++current;
    }

    return cache[n - 1];
}
