/*
 * A116050  n+sigma(n)+sigma(sigma(n)) is a prime.
 * https://oeis.org/A116050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 571, u32 n <= 571, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n‑th integer m such that m + σ(m) + σ(σ(m)) is a prime
   (offset 1) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;      /* allocated cache size            */
static idx_t cache_count = 0;     /* number of terms stored          */
static idx_t current_test = 1;    /* next integer to test for property */

seq_t a(idx_t n) {
    /* Compute terms until we have at least n of them. */
    while (cache_count < n) {
        /* Evaluate σ(current_test) and σ(σ(current_test)) */
        seq_t sigma_cur   = sigma_of((idx_t)current_test);
        seq_t sigma_sigma = sigma_of((idx_t)sigma_cur);

        /* Check whether m + σ(m) + σ(σ(m)) is prime. */
        seq_t sum = (seq_t)current_test + sigma_cur + sigma_sigma;
        if (is_prime_big(sum)) {
            /* Grow cache if necessary. */
            if (cache_count >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count] = (seq_t)current_test;
            cache_count++;
        }

        current_test++; /* move to the next integer */
    }

    /* Return the n‑th element (1‑based). */
    return cache[n - 1];
}
