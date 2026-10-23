/*
 * A140542  Primes of form 17*n - 6.
 * https://oeis.org/A140542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form 17*k-6: a(n) is the n‑th such prime. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Cached terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;   /* last k tested */

    /* Return immediately if already cached */
    if (n <= cache_len)
        return cache[n - 1];

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely in tests */
    }

    /* Generate terms until we reach the requested index */
    while (cache_len < n) {
        ++last_k;
        seq_t candidate = (seq_t)17 * (seq_t)last_k - 6;

        if (isPrimeGeneric(candidate)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = candidate;
        }
    }

    return cache[n - 1];
}
