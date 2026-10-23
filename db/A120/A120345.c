/*
 * A120345  Numbers n such that 2357*n + 1 is prime.
 * https://oeis.org/A120345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120345:
   a(n) = the n‑th positive integer m such that 2357*m+1 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n==0, but safe */

    static seq_t *cache = NULL;          /* stored terms          */
    static size_t len   = 0;             /* number of terms stored*/
    static size_t cap   = 0;             /* allocated capacity    */
    static uint64_t next_m = 1;          /* next m to test        */

    if ((idx_t)len >= n)                 /* already computed      */
        return cache[(size_t)(n - 1)];

    /* compute missing terms up to n */
    for (size_t i = len; i < (size_t)n; ++i)
    {
        uint64_t m = next_m;
        seq_t val;

        /* find the next m with 2357*m+1 prime */
        for (;; ++m)
        {
            val = (seq_t)(2357 * m + 1);
            if (is_prime_big(val))
                break;
        }
        next_m = m + 1;                 /* next search starts after m */

        /* grow the cache if needed */
        if (len == cap)
        {
            size_t new_cap = cap == 0 ? 4 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = (seq_t)m;        /* store the term */
    }

    return cache[(size_t)(n - 1)];
}
