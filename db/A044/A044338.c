/*
 * A044338  Numbers n such that string 0,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   has06_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: scan natural numbers until the nth one that contains the
   substring "06" but whose predecessor does not.  This algorithm is
   linear in the size of the answer, which is well below 2 s for n≈10 000. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has06_uint64.h"

/* Cached sequence values.  Static state allows unordered calls. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;            /* number of terms stored */
static uint64_t search_val = 0;         /* next integer to test */

seq_t a(idx_t n)
{
    /* Grow cache until it contains the nth term. */
    while (cache_len < (size_t)n) {
        /* Find the next integer that satisfies the rule. */
        do {
            search_val++;
        } while (!(has06_uint64(search_val) && !has06_uint64(search_val - 1)));

        /* Store the found term into the cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)                /* allocation failure – stop execution */
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)search_val;
    }
    return cache[(size_t)n - 1];
}
