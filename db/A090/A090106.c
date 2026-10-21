/*
 * A090106  Values of k such that {P(k), P(k+1), ..., P(k+12)} are all prime
 *          numbers, where P(k) = k^2 + k + 41.
 * https://oeis.org/A090106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A090106:  k such that P(k)=k²+k+41, …, P(k+12) are all prime.
   The function returns the k‑th such value (a(1)=1). It uses a simple
   brute‑force search with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of found k-values (stored as seq_t). */
    static seq_t *cache     = NULL; /* array of values */
    static size_t cache_len = 0;    /* number of cached entries      */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t next_k = 1;        /* next k to test               */

    /* Grow the cache until it contains at least n entries. */
    while (cache_len < n) {
        bool all_prime = true;

        /* Test the 13 values of P(k+j) for j = 0 … 12. */
        for (int j = 0; j <= 12 && all_prime; ++j) {
            idx_t x = next_k + j;

            /* Compute P(x) = x² + x + 41 using seq_t arithmetic.
               This works for both built‑in integer types and for the
               arbitrary‑precision seq_t classes that overload + and *.
            */
            seq_t sx   = (seq_t)x;
            seq_t val  = sx * sx + sx + (seq_t)41;

            if (!is_prime_big(val))
                all_prime = false;
        }

        if (all_prime) {
            /* Ensure the cache has space for the new entry. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)
                    abort();            /* allocation failure */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
