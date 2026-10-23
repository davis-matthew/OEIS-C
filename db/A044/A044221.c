/*
 * A044221  Numbers n such that string 4,2 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_42_in_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th integer whose base‑8 representation
   contains the substring “42” while the preceding integer does not.
   The function searches forward from 1, building the list as needed,
   and memoises previously found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_42_in_base8.h"

static seq_t *cache = NULL;        /* array of found terms          */
static idx_t   cache_len = 0;       /* number of terms stored       */
static idx_t   cache_cap = 0;       /* allocated capacity           */
static seq_t   last_checked = 1;    /* next natural number to test  */

seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (cache_len == 0 && cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Keep generating until we have the n‑th term */
    while (cache_len < n) {
        seq_t candidate = last_checked;

        /* Check the two conditions */
        if (has_42_in_base8(candidate) &&
            !has_42_in_base8(candidate - 1)) {

            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }

        last_checked++;
    }

    return cache[n - 1];
}
