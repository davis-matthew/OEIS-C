/*
 * A227793  Numbers whose digital sum is a multiple of 5.
 * https://oeis.org/A227793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose digit sum is a multiple of 5
   The nth term a(n) is the nth non‑negative integer whose decimal
   digit sum is ≡ 0 (mod 5).  The function builds a minimal cache
   of computed terms and extends it when necessary. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t) */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;     /* pointer to the cache array   */
    static size_t cache_len = 0;    /* number of terms stored      */
    static size_t cache_cap = 0;    /* allocated capacity          */
    static seq_t last_term = 0;     /* the last integer examined   */

    if (n == 0)                     /* undefined; return 0 safely */
        return (seq_t)0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 1;
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)                    /* allocation failure */
            return (seq_t)0;
        cache[0] = 0;                  /* a(1) = 0 */
        last_term = 0;                 /* last integer examined */
    }

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Extend the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        last_term = last_term + 1;    /* next integer to test */

        /* Check if digit sum is a multiple of 5 */
        if ((digit_sum(last_term) % 5u) == 0u) {
            /* Ensure sufficient capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)                    /* allocation failure */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_term;
        }
    }

    return cache[n - 1];
}
