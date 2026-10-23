/*
 * A028839  Sum of digits of n is a square.
 * https://oeis.org/A028839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1242, u32 n <= 1242, u64 n <= 1242,
 *            u128 n <= 1242, big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose digit sum is a perfect square
   Formula: a(n) = the n‑th positive integer whose decimal digit sum
   is a perfect square (a(1)=1, a(2)=4, …) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t v);           */
#include "../utils/floor_sqrt.h"  /* seq_t floor_sqrt(seq_t x);          */

seq_t a(idx_t n)
{
    /* --- static cache --------------------------------------------------- */
    static seq_t *cache      = NULL;   /* array of found terms           */
    static size_t cache_len  = 0;       /* number of cached terms         */
    static size_t cache_cap  = 0;       /* allocated capacity of cache    */
    static seq_t candidate   = 1;       /* next integer to test           */

    /* --- fill cache until it contains at least n terms ----------------- */
    while (cache_len < (size_t)n) {
        /* enlarge capacity if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failed */
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        /* test current candidate */
        seq_t sum  = digit_sum(candidate);
        seq_t root = floor_sqrt(sum);
        if (root * root == sum) {
            cache[cache_len++] = candidate;
        }

        /* advance to the next integer */
        candidate++;
    }

    return cache[n - 1];   /* n≥1, so this is valid */
}
