/*
 * A201850  Primes of the form 7n^2 - 4.
 * https://oeis.org/A201850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 7·n²−4 (n ≥ 1), 1‑based
 *            indexing
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 1085, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"

/*  seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined; avoid UB if called with 0 */

    static seq_t *prime_cache = NULL;
    static size_t cache_size = 0;   /* number of cached primes */
    static size_t cache_cap  = 0;   /* allocated capacity of the cache */
    static idx_t next_k = 1;        /* next k to test for 7k²-4 */

    while (cache_size < (size_t)n)
    {
        /* Compute the candidate 7·k²−4 using a wider type to avoid
           overflow in seq_t before the final cast. */
        unsigned long long k = (unsigned long long)next_k;
        unsigned long long val_ul = 7ULL * k * k - 4ULL;

        seq_t candidate = (seq_t)val_ul;

        if (isPrimeGeneric(candidate))
        {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);        /* abort on OOM */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_size++] = candidate;
        }
        ++next_k;
    }

    return prime_cache[(size_t)(n - 1)];  /* adjust for 1‑based index */
}
