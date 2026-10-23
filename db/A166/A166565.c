/*
 * A166565  Primes p such that product of digits +- 1 is prime.
 * https://oeis.org/A166565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime p such that product of digits of p ± 1 are both
 *            prime
 * Range:     u8 n <= 11, u16 n <= 830, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, digit_product, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation incrementally searches primes and stores the ones
   satisfying the definition in a static cache.  It uses the helper
   functions
   - digit_product() from "../utils/digit_product.h",
   - is_prime_small() from "../utils/is_prime_small.h",
   - append_candidate() from "../utils/append_candidate.h".
   The cache allows the function to answer calls in any order and
   keeps previous results so subsequent calls are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>                        /* for uint64_t */
#include "../utils/digit_product.h"
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

/* static data for incremental search and caching */
static uint64_t last_checked = 1;          /* last number examined for primality   */
static size_t  found_count = 0;            /* number of sequence terms found so far */
static seq_t *cache_seq = NULL;            /* dynamic array of found primes        */
static size_t cache_len = 0;               /* current number of cached terms       */
static size_t cache_cap = 0;               /* current capacity of cache_seq        */

/* Find the next prime > x using a simple incremental trial divisor.
   That is sufficiently fast for the required range (up to a few million). */
static uint64_t next_prime(uint64_t x)
{
    if (x < 2) return 2;
    if (x == 2) return 3;
    uint64_t p = (x % 2 == 0) ? x + 1 : x + 2;   /* start with an odd number */

    for (;;) {
        if (is_prime_small((seq_t)p))
            return p;
        p += 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, but return 0 safely */

    /* Expand cache until we have at least n terms */
    while (found_count < (size_t)n) {
        uint64_t cand = next_prime(last_checked);
        last_checked = cand;

        seq_t prod = digit_product((seq_t)cand);
        /* product of digits is always positive; add 1 and -1 are safe */
        if (is_prime_small(prod + 1) && is_prime_small(prod - 1)) {
            append_candidate(&cache_seq, &cache_len, &cache_cap,
                             (seq_t)cand);
            ++found_count;
        }
    }

    /* n is 1‑based and cache is 0‑based */
    return cache_seq[n - 1];
}
