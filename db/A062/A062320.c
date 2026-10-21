/*
 * A062320  Nonsquarefree numbers squared. A013929(n)^2.
 * https://oeis.org/A062320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 98, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062320: nonsquarefree numbers squared.
   Computed by scanning integers and testing squarefree. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* static cache for already computed squares */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;
    static idx_t  last_checked = 0;  /* highest integer examined so far */

    /* extend cache until it contains n elements */
    if (cache_len < n) {
        /* ensure there is room for at least one more entry before allocation */
        while (cache_len < n) {
            idx_t candidate = last_checked + 1;  /* next integer to test */

            /* test whether candidate is not squarefree */
            if (!is_squarefree((seq_t)candidate)) {
                /* compute and store its square */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;          /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)candidate * (seq_t)candidate;
            }
            last_checked = candidate;          /* advance the last_checked marker */
        }
    }
    return cache[n - 1];
}
