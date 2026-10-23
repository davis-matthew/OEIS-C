/*
 * A162293  Numbers k such that k^2*(k-1)-1 is prime.
 * https://oeis.org/A162293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 531, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for k such that k^2*(k-1)-1 is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset not used */

    /* static cache of values to speed up repeated queries */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* capacity of the cache */
    static idx_t next_k = 2;        /* next k to test */

    /* extend the cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t expr = k * k * (k - 1) - 1;
        if (isPrimeGeneric(expr)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
        next_k++;
    }

    return cache[n - 1];
}
