/*
 * A087103  Smallest jumping champion for prime(n).
 * https://oeis.org/A087103
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest jumping champion for prime(n).
   The function generates all primes up to the nth prime,
   counts the differences between consecutive primes,
   and returns the smallest gap that occurs most often. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 2; below that we return 0. */
    if (n < 2)
        return 0;

    /* 1. Find the nth prime. */
    uint32_t pn = get_prime(n);
    if (pn < 2)   /* safety check, should never happen for n >= 2 */
        return 0;

    /* 2. Generate all primes up to that prime. */
    uint64_t *primes_raw = NULL;      /* array of primes (uint64_t) */
    size_t prime_cnt = 0;             /* number of primes */
    build_primes_upto((idx_t)pn, &primes_raw, &prime_cnt);

    if (prime_cnt < 2) {
        free(primes_raw);
        return 0;
    }

    /* 3. Find the largest prime gap to size the frequency table. */
    uint64_t max_gap = 0;
    for (size_t i = 1; i < prime_cnt; ++i) {
        uint64_t g = primes_raw[i] - primes_raw[i - 1];
        if (g > max_gap)
            max_gap = g;
    }

    /* 4. Allocate frequency array for gaps (size max_gap + 1). */
    size_t freq_size = (size_t)max_gap + 1;   /* gaps start at 1 */
    uint32_t *freq = (uint32_t *)calloc(freq_size, sizeof(uint32_t));
    if (!freq) {  /* allocation failed – return 0 as a safe fallback */
        free(primes_raw);
        return 0;
    }

    /* 5. Count the gaps. */
    for (size_t i = 1; i < prime_cnt; ++i) {
        uint64_t g = primes_raw[i] - primes_raw[i - 1];
        /* g is <= max_gap, so array bounds are safe */
        ++freq[(size_t)g];
    }

    /* 6. Determine the smallest gap with the maximum frequency. */
    uint32_t best_gap = 0;
    uint32_t best_count = 0;
    for (size_t g = 1; g < freq_size; ++g) {
        uint32_t c = freq[g];
        if (c > best_count) {
            best_count = c;
            best_gap = (uint32_t)g;
        } else if (c == best_count && c > 0 && g < best_gap) {
            best_gap = (uint32_t)g;
        }
    }

    /* 7. Clean up and return the result. */
    free(freq);
    free(primes_raw);
    return (seq_t)best_gap;
}
