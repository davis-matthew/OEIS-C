/*
 * A176069  Numbers of the form k^2+k+1 that are the product of two distinct
 *          primes.
 * https://oeis.org/A176069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 116, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers of the form k^2+k+1 that are the product of two
   distinct primes (OEIS A176069). */

/* The algorithm simply scans increasing k, computes k^2+k+1 and tests
   whether it has exactly two prime factors, both of multiplicity one.
   Results are cached so that repeated calls for increasing or
   arbitrary indices are efficient. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Cached results */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* current capacity of cache array */

    /* Progress along k */
    static idx_t last_k = 0;         /* last k examined */

    /* Undefined for n < 1; return 0 as a harmless value */
    if (n < 1) return (seq_t)0;

    /* Extend cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Next k */
        last_k++;

        /* Compute k^2 + k + 1; use seq_t to avoid overflow */
        seq_t val = (seq_t)last_k * (seq_t)last_k + (seq_t)last_k + (seq_t)1;

        /* Factor the value using the provided helper.
           factor_u64 accepts 64‑bit numbers, and the values in this
           sequence for n ≤ 10000 are far below 2^64. */
        uint64_t primes[8];
        size_t  exp[8];
        size_t  cnt;
        factor_u64((uint64_t)val, primes, exp, &cnt);

        /* Check for a product of two distinct primes. */
        if (cnt == 2 && exp[0] == 1 && exp[1] == 1) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure: abort */
                    exit(1);
                }
                cache    = tmp;
                cache_cap = new_cap;
            }
            /* Store the found term */
            cache[cache_len++] = val;
        }
    }

    /* Return the requested n‑th term (1‑based). */
    return cache[(size_t)n - 1];
}
