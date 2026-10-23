/*
 * A129079  Prime numbers that are the sum of consecutive prime numbers with
 *          the final digit 7 (primes in A030432).
 * https://oeis.org/A129079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A129079
 *
 * a(n) is the nth prime that can be expressed as the sum of the first k
 * consecutive primes that end with the digit 7 (sequence A030432).
 * The definition is implemented by generating primes of the form 10k+7,
 * adding them to a running cumulative sum, and recording that sum
 * whenever it is itself prime.
 *
 * First few terms:
 *   a(1) = 7
 *   a(2) = 61
 *   a(3) = 379
 *   ...
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"       /* int is_prime(idx_t x)   */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache      = NULL;   /* array of earlier a(k) */
    static size_t cache_len  = 0;      /* number of cached terms */
    static size_t cache_cap  = 0;      /* allocated capacity */

    /* Working state for sum construction */
    static seq_t cumulative_sum = 0;   /* current sum of primes ending with 7 */
    static uint64_t candidate    = 0;  /* next number of form 10k+7 to test */
    static bool initialized      = false;  /* flag for one‑time init */

    /* One‑time initialisation */
    if (!initialized) {
        cache_cap = 16;                          /* initial capacity */
        cache     = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache_len = 0;
        cumulative_sum = 0;
        candidate = 7;                           /* first prime ending with 7 */
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Does candidate itself end with 7 and is prime? */
        if (is_prime(candidate)) {
            /* Add to cumulative sum */
            cumulative_sum += (seq_t)candidate;

            /* If the sum is prime, record it */
            if (is_prime_big(cumulative_sum)) {
                /* Grow cache if necessary */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    if (new_cap == 0) new_cap = 16;
                    while (new_cap <= cache_len) new_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cumulative_sum;
            }
        }

        /* Advance to the next candidate ending with 7 */
        candidate += 10;
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
