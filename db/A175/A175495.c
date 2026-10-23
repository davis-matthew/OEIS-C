/*
 * A175495  Positive integers k such that k < 2^d(k), where d(k) is the number
 *          of divisors of k.
 * https://oeis.org/A175495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A175495.
   The sequence consists of integers k with k < 2^d(k),
   where d(k) is the number of divisors of k.
   A memoised linear sieve is used: each new integer is tested
   until the cache holds at least n terms.  The test is
   performed by the helper function divisor_count_u64().
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Static cache of all terms found so far.
       cache_len  – current number of cached terms
       cache_cap  – current capacity of the cache array
       next_k     – next integer to test for inclusion
    */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;

    /* Expand the cache until we have at least n terms. */
    while (cache_len < n) {
        uint64_t d = divisor_count_u64((uint64_t)next_k);

        bool passes;
        if (d >= 64) {
            /* 2^d exceeds any 64‑bit integer, so the inequality always holds. */
            passes = true;
        } else {
            /* For d < 64 we can shift safely in idx_t. */
            passes = next_k < ((idx_t)1 << d);
        }

        if (passes) {
            /* Ensure there is space for the new term. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – unlikely, return 0 as a safe fallback. */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }

        ++next_k;
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
