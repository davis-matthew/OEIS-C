/*
 * A131210  Numbers k such that 24k - 1 is prime.
 * https://oeis.org/A131210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 780, u32 n <= 781, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 24*k - 1 is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Return the n-th positive integer k for which 24*k-1 is prime.
   The sequence is indexed starting at 1. */
seq_t a(idx_t n)
{
    /* Cache of previously found terms.  cache[i] holds the (i+1)-th term. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    if (n == 0)            /* undefined for offset 0 – return 0 as a sentinel */
        return 0;

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Start searching from the integer just beyond the last cached term. */
    seq_t k = (cache_size == 0) ? (seq_t)1 : (seq_t)cache[cache_size - 1] + (seq_t)1;

    /* Grow the cache as needed. */
    while (cache_size < (size_t)n) {
        seq_t val = ((seq_t)24) * k - (seq_t)1;
        if (is_prime_big(val)) {
            /* Ensure capacity. */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
        ++k;
    }

    return cache[n - 1];
}
