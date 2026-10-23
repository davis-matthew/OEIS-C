/*
 * A116035  Numbers k such that k + phi(k) + sigma(k) is a prime.
 * https://oeis.org/A116035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k + phi(k) + sigma(k) is prime (OEIS A116035). */
/* The function iteratively finds k's that satisfy the condition, caches
   them, and returns the nth such k (1‑based).  The algorithm uses
   calc_phi, sigma_of, and is_prime_big from the utilities. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper prototypes */
#include "../utils/calc_phi.h"    /* seq_t calc_phi(seq_t m); */
#include "../utils/sigma_of.h"    /* seq_t sigma_of(idx_t n); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* Items are defined only for n >= 1.  For n < 1 simply return 0. */
    if (n < 1)
        return 0;

    /* Static cache of the found terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t next_k = 1;      /* next k to test */

    /* Keep generating until we have at least n terms */
    while (cache_len < n) {
        /* Compute the value k + phi(k) + sigma(k) */
        idx_t k_i = next_k;          /* integer k */
        seq_t k = (seq_t)k_i;        /* convert to seq_t for arithmetic */

        seq_t sum = k + calc_phi(k) + (seq_t)sigma_of(k_i);

        if (is_prime_big(sum)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
