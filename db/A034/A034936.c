/*
 * A034936  Numbers k such that 3*k + 4 is prime.
 * https://oeis.org/A034936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that 3*k + 4 is prime
 * Range:     u8 n <= 25, u16 n <= 3439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1 */

    /* static cache for the computed k‑values */
    static seq_t *cache = NULL;        /* array of found k’s */
    static size_t cache_size = 0;      /* number of elements stored */
    static size_t cache_cap   = 0;      /* current capacity of the array */

    /* state for incremental generation */
    static idx_t next_k = 1;           /* next integer to test */
    static idx_t found = 0;            /* how many terms have been found */

    /* extend the cache until it contains at least n terms */
    while (found < n)
    {
        idx_t cand = 3 * next_k + 4;   /* candidate prime 3*k + 4 */
        if (is_prime_small((seq_t)cand))
        {
            /* grow the cache if necessary */
            if (cache_size == cache_cap)
                ensure_capacity(cache_size + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));

            cache[cache_size++] = (seq_t)next_k;
            ++found;
        }
        ++next_k;                     /* advance to next candidate */
    }

    /* return the n‑th term (1‑based index) */
    return cache[n - 1];
}
