/*
 * A165503  Primes p with a digits sum of p^2 equal to 37.
 * https://oeis.org/A165503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 773, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, square_digit_sum_is_37
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that the sum of the decimal digits of p² equals 37. */
/* The sequence is indexed from 1.  The implementation searches successive primes
   and stores found terms in a static cache, so subsequent calls do not repeat work. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/square_digit_sum_is_37.h"

/* The main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1; return 0 */

    /* Static cache for memoised terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_candidate = 2; /* number to start next search from */

    /* Expand the cache until it contains at least n elements. */
    while (cache_len < n) {
        /* Find the next prime >= next_candidate. */
        idx_t p = next_prime(next_candidate);

        /* Check the digit‑sum condition. */
        if (square_digit_sum_is_37(p)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = (seq_t)p;  /* store the found term */
            next_candidate = p + 1;         /* start next search after this prime */
        } else {
            /* Continue searching after this prime. */
            next_candidate = p + 1;
        }
    }

    return cache[n - 1]; /* 1‑based indexing */
}
