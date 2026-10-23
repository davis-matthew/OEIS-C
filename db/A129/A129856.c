/*
 * A129856  Primes that are one less than the difference between consecutive
 *          primes.
 * https://oeis.org/A129856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A129856 – primes that are one less than the difference between
 *            consecutive primes. The function generates successive prime
 *            gaps, checks if the gap minus one is prime, and records that
 *            prime. Results are cached so that calls may be in any order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* If n is below the OEIS offset, return zero. */
    if (n == 0) return (seq_t)0;

    /* Cache of sequence values. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    /* State for successive prime generation. */
    static uint64_t prev_prime   = 2; /* prime to start after */
    static bool initialized      = false;

    if (!initialized) {
        initialized = true;
    }

    /* Expand cache capacity if necessary. */
    if (size < n && cap < (size_t)n) {
        size_t new_cap = (size_t)n * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        uint64_t next_p = next_prime(prev_prime);
        uint64_t diff   = next_p - prev_prime;
        prev_prime      = next_p;

        if (diff <= 2) continue;           /* diff-1 would not be prime */

        seq_t cand = (seq_t)(diff - 1);
        if (is_prime_small(cand)) {
            cache[size++] = cand;
        }
    }

    return cache[n - 1];
}
