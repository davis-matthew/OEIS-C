/*
 * A139487  Numbers k such that 8k + 7 is prime.
 * https://oeis.org/A139487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 8k + 7 is prime.  a(1) = 0. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Lazy memoization: cache successive solutions k */
    static seq_t *cache = NULL;   /* stored k values (converted to seq_t) */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t size  = 0;      /* number of stored values */
    static idx_t  last_k = (idx_t)-1;   /* candidate k to test next */

    if (n == 0)      /* position is 1‑based; 0 is undefined */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (size < n)
    {
        last_k += 1;                          /* next k to test         */
        idx_t val = last_k * 8 + 7;           /* 8k+7                      */

        /* Primality test using the utility for 64‑bit integers. */
        if (is_prime(val))                    /* 8k+7 is prime?   */
        {
            /* Grow the cache if needed. */
            if (size >= cap)
            {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;           /* allocation failure  */
                cache = tmp;
                cap   = new_cap;
            }
            cache[size++] = (seq_t)last_k;    /* store the value k   */
        }
    }

    return cache[n - 1];   /* zero‑based indexing in the cache */
}
