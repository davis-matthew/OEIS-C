/*
 * A044678  Numbers n such that string 5,1 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains51
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑9 representation of n contains the
   substring "51" but the base‑9 representation of n+1 does not.
   The function scans integers in increasing order, caching results so that
   any call a(idx_t n) returns the nth term (a(1) is the first such number). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains51.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static seq_t current = 1;      /* next integer to test */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Check if current satisfies the property. */
        seq_t next = current + 1;
        if (contains51(current) && !contains51(next)) {
            /* Store it. */
            if (cache_len == cache_cap) {
                size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = current;
        }
        current++;   /* advance to next integer */
    }

    return cache[(size_t)n - 1];
}
