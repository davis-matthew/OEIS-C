/*
 * A044722  Numbers n such that string 0,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   has_09
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements R. 71485791: a(n) is the n‑th positive integer whose
 * decimal representation contains the substring "09" but the next
 * integer n+1 does not.  The sequence is indexed with a(1)=109. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_09.h"

/* Return the n-th term (n >= 1).  Calls may arrive in arbitrary order. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of found terms      */
    static size_t cache_cap  = 0;      /* allocated capacity        */
    static size_t cache_used = 0;      /* number of terms stored    */
    static seq_t next_candidate = (seq_t)1; /* integer being tested    */

    if (n == 0)      /* undefined for offset 1; return 0 safely */
        return (seq_t)0;

    /* Ensure the cache can hold at least n terms.  Grow exponentially
     * to keep allocations O(log n). */
    while (cache_used < (size_t)n) {
        /* Grow cache if necessary */
        if (cache_used == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                /* allocation failure   */
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Test the current candidate */
        if (has_09(next_candidate) && !has_09(next_candidate + (seq_t)1))
            cache[cache_used++] = next_candidate;

        next_candidate++;
    }

    return cache[(size_t)n - 1];
}
