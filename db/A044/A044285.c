/*
 * A044285  Numbers n such that string 3,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has37_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044285: numbers n such that the base‑9 representation of
   n contains the substring 3,7 but the representation of n‑1 does not.
   The function enumerates such numbers incrementally, caching all
   values seen so far so calls can be in any order. */
/* Note: no closed formula is known; the definition is checked by brute
   force. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has37_base9.h"

/* Cache of already computed terms and auxiliary state.  The cache is
   enlarged as needed; last_checked holds the greatest integer that
   has been examined so far (including numbers that did not qualify). */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t last_checked = 0;

/* Return the nth term of A044285 (1‑based index).  Calls come in any
   order; the routine fills the cache lazily. */
seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; behaviour for n==0 is unspecified, return 0 */
        return (seq_t)0;

    /* Fast return if already computed. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Ensure the cache can hold the new entries. */
    while (cache_len < (size_t)n) {
        /* Grow the cache, doubling its capacity. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            /* Explicit cast required for C++ compatibility. */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – terminate as per typical lib usage. */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Scan positive integers until we find the next qualifying number. */
        seq_t candidate = last_checked + 1;
        while (1) {
            if (has37_base9(candidate) && !has37_base9(candidate - 1)) {
                /* Store the new term. */
                cache[cache_len] = candidate;
                ++cache_len;
                last_checked = candidate;
                break;      /* move on to fill the next required position */
            }
            ++candidate;
            last_checked = candidate - 1;
        }
    }

    return cache[(size_t)n - 1];
}
