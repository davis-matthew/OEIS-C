/*
 * A141988  Primes congruent to 12 mod 29.
 * https://oeis.org/A141988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 233, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 12 mod 29 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* The following function returns the n-th prime p with p % 29 == 12.
   It uses a simple sieve and caches the results so that arbitrary
   (non‑increasing) calls are fast. */
seq_t a(idx_t n)
{
    /* cache of found primes (uint64_t).  Every call converts the cached
       value to seq_t when it is returned. */
    static uint64_t *cand = NULL;
    static size_t cand_len = 0;
    static idx_t cur_limit = 0;      /* highest number already sieved */

    if (n == 0)
        return 0;                    /* safety: offset is 1 */

    /* If we already have enough primes, just return the requested one. */
    if (cand_len >= (size_t)n)
        return (seq_t)cand[n - 1];

    /* Grow the sieve limit until we have at least n terms. */
    idx_t new_limit = cur_limit ? cur_limit * 2 : 100;
    while (1) {
        /* Allocate sieve array: false = prime (unknown), true = composite. */
        bool *sieve = (bool *)calloc((size_t)new_limit + 1, sizeof(bool));
        if (!sieve)
            return 0;                 /* allocation error → dummy value */

        /* Sieve of Eratosthenes. */
        for (idx_t i = 2; i <= new_limit / i; ++i) {
            if (!sieve[i]) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    sieve[j] = true;
            }
        }

        /* Count how many new primes will be stored. */
        size_t new_len = 0;
        for (idx_t p = 2; p <= new_limit; ++p)
            if (!sieve[p] && (p % 29 == 12))
                ++new_len;

        /* Allocate array for primes.  Use uint64_t so we can store them
           independently of how seq_t is defined. */
        uint64_t *new_arr = (uint64_t *)malloc(new_len * sizeof(uint64_t));
        if (!new_arr) {
            free(sieve);
            return 0;
        }

        /* Fill the array. */
        size_t pos = 0;
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (!sieve[p] && (p % 29 == 12))
                new_arr[pos++] = (uint64_t)p;
        }

        free(sieve);
        free(cand);        /* discard previous cache */
        cand = new_arr;
        cand_len = new_len;
        cur_limit = new_limit;

        if (cand_len >= (size_t)n)
            break;          /* enough primes collected */

        new_limit *= 2;     /* increase search bound and retry */
    }

    /* Return the n‑th term, converting to seq_t. */
    return (seq_t)cand[n - 1];
}
