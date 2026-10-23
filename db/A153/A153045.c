/*
 * A153045  Numbers k such that 2*k-11 is not a prime.
 * https://oeis.org/A153045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers k such that 2*k-11 is a composite integer >=4
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates the sequence on demand using a simple sieve-based prime test. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_k = 2;       /* next k to test */

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow cache in powers of two for efficiency. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the cache until we have enough terms. */
    while (cache_len < n) {
        /* Find the next k satisfying the condition. */
        for (;;) {
            if (next_k < 6) {  /* 2*k-11 < 4 for k <= 5, so skip */
                ++next_k;
                continue;
            }

            seq_t y = ((seq_t)next_k << 1) - 11;  /* y = 2*k - 11 */

            /* y must be at least 4 and not prime. */
            if (y >= 4 && !is_prime((idx_t)y))
                break;
            ++next_k;
        }

        /* Ensure capacity for the new term. */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Store the term and prepare for the next iteration. */
        cache[cache_len++] = next_k;
        ++next_k;
    }

    return cache[n - 1];
}
