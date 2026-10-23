/*
 * A167692  The even nonisolated nonprimes.
 * https://oeis.org/A167692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*----- OEIS A167692 ------------------------------------------------------------
 * Even nonisolated nonprimes: a(1)=0 and for n≥2, a(n) is the (n‑1)‑th even
 * integer n such that at least one of n‑1 or n+1 is composite.
 * Implementation iteratively tests consecutive even numbers and caches
 * the result for fast repeated queries.
 ---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t cache_cap = 0;         /* capacity of array   */
    static size_t cache_len = 0;         /* number of terms stored (≥1) */
    static seq_t next_even = 8;          /* next even number to test */

    /* Initialise the cache with the first term a(1)=0 */
    if (cache_len == 0) {
        cache_cap = 16;                       /* initial capacity */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();                  /* out of memory */
        cache[0] = 0;                         /* a(1) = 0 */
        cache_len = 1;
    }

    /* Generate terms until the requested index is reached */
    while (cache_len < (size_t)n) {
        /* Test the current even number */
        bool left_prime  = is_prime_big(next_even - 1);
        bool right_prime = is_prime_big(next_even + 1);

        if (!(left_prime && right_prime)) {    /* at least one composite */
            if (cache_len + 1 > cache_cap) {   /* grow cache if needed */
                size_t new_cap = cache_cap * 2U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_even;     /* store the term */
        }

        next_even += 2;                        /* move to next even */
    }

    /* Return the n‑th term (1‑based indexing) */
    return cache[n - 1];
}
