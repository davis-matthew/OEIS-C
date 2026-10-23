/*
 * A044406  Numbers n such that string 7,4 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_74
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑10 representation of n contains the substring "74" but n‑1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_74.h"

static seq_t *cache    = NULL;
static size_t  cache_size = 0;
static size_t  cache_cap   = 0;
static seq_t  last_value    = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset */

    /* If the value has already been computed, return it directly. */
    if ((size_t)n - 1 < cache_size)
        return cache[(size_t)n - 1];

    /* Compute terms until we have the nth term. */
    while (cache_size < (size_t)n) {
        last_value += 1;                  /* next integer to test */

        if (contains_74(last_value)) {
            bool prev_has = false;
            if (last_value > (seq_t)0)
                prev_has = contains_74(last_value - 1);

            if (!prev_has) {
                /* Grow the cache if necessary. */
                if (cache_size >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (cache == NULL)
                                 ? (seq_t *)malloc(sizeof(seq_t) * new_cap)
                                 : (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = last_value;
            }
        }
    }
    return cache[(size_t)n - 1];
}
