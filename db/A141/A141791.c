/*
 * A141791  Primes of the form c(n)-n, where c(n)=n-th composite.
 * https://oeis.org/A141791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime numbers of the form c(n)-n, where c(n) is the n‑th
 *            composite number
 * Range:     u8 n <= 319, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* state for generating terms lazily */
    static idx_t cursor   = 4; /* next integer to inspect (first composite) */
    static idx_t comp_idx = 0; /* count of composites seen */

    /* ensure enough capacity */
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* advance cursor to the next composite */
        while (is_prime(cursor)) ++cursor;
        ++comp_idx; /* cursor now points to the comp_idx‑th composite */

        /* diff = c(k) - k */
        seq_t diff = (seq_t)cursor - (seq_t)comp_idx;

        /* store if diff is prime */
        if (isPrimeGeneric(diff)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) return (seq_t)0;
            }
            cache[cache_len++] = diff;
        }
        ++cursor; /* move to next integer for next iteration */
    }

    return cache[n - 1];
}
