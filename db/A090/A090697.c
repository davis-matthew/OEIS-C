/*
 * A090697  Numbers n such that n^2/2 - 1 is a prime.
 * https://oeis.org/A090697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth positive integer i such that i^2/2 – 1 (with i
 *            even) is a prime. In other words a(n) is the nth integer n for
 *            which (n^2)/2‑1 is prime. The sequence begins with a(1)=4,
 *            a(2)=6, a(3)=8, …
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) – deterministic for 64‑bit */

seq_t a(idx_t n)
{
    /* Cache of already-found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms stored   */
    static size_t cache_cap = 0;   /* allocated capacity       */

    /* Current even candidate to test.  Start from the first
       number that can possibly satisfy the condition. */
    static seq_t cand = (seq_t)4;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* p = cand^2 / 2 – 1.  All operations are in seq_t. */
        seq_t sq = cand * cand;
        seq_t p  = sq / (seq_t)2 - (seq_t)1;

        /* Test primality.  The values needed for the tests
           fit into a 64‑bit integer for the ranges used by
           the project (n ≲ 10⁵ → p ≲ 5·10¹⁰). */
        if (is_prime((idx_t)p))
        {
            /* Grow the cache array if needed. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* unrecoverable allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        cand += (seq_t)2;   /* only even numbers can satisfy the condition */
    }

    return cache[n - 1];
}
