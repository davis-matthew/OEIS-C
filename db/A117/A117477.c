/*
 * A117477  Primes whose SOD and that of their indices are both prime and
 *          equal (indices may not be prime, but their SOD must be prime).
 * https://oeis.org/A117477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A117477: primes p for which the sum of digits of p is prime
 * and equals the sum of digits of the index of p in the list of all primes.
 * The function searches primes incrementally, memoises the results
 * and works for any build of seq_t. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Sum of decimal digits of a 64‑bit integer. */
static inline uint32_t digit_sum_u64(uint64_t x)
{
    uint32_t s = 0;
    while (x) {
        s += (uint32_t)(x % 10);
        x /= 10;
    }
    return s;
}

/* Test whether a small integer is prime using the generic big‑prime test. */
static inline bool is_small_prime_uint32(uint32_t n)
{
    return is_prime_big((seq_t)n);
}

/* Cached results of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Current state of the prime sieve used in the search. */
static idx_t  candidate      = 2;   /* next integer to test */
static uint64_t prime_rank = 0;      /* number of primes seen so far */

/* Retrieve the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined – fall back to zero */

    /* Initialise cache on first use. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        if (is_prime(candidate)) {             /* candidate is a prime */
            ++prime_rank;                       /* its index in the prime list */

            uint32_t sumP = digit_sum_u64((uint64_t)candidate);
            uint32_t sumI = digit_sum_u64(prime_rank);

            if (sumP == sumI && is_small_prime_uint32(sumP)) {
                if (cache_len == cache_cap) {   /* grow cache if needed */
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)candidate;  /* store the term */
            }
        }

        /* Advance to the next integer, skipping even numbers after 2. */
        candidate++;
        if (candidate > 2 && (candidate & 1) == 0)
            candidate++;
    }

    return cache[n - 1];
}
