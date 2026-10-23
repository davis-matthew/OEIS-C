/*
 * A044796  Numbers k such that string 8,3 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k such that the decimal representation of k
 *            contains the substring "83" but the decimal representation of
 *            k+1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_83
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_83.h"

/* memoised sequence storage */
static seq_t *cache      = NULL;   /* array of found terms */
static size_t  cache_len  = 0;     /* number of stored terms */
static size_t  cache_cap  = 0;     /* current capacity of cache */
static seq_t   next_check = (seq_t)1; /* next integer to test */

/* the required function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined for n < 1, return 0 as a placeholder */
        return (seq_t)0;

    /* grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k = next_check;
        for (;;) {
            if (contains_83(k) && !contains_83(k + (seq_t)1)) {
                /* store the found term */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0 ? 4 : cache_cap << 1);
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache    = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                next_check = k + (seq_t)1; /* start the next search after this k */
                break;
            }
            ++k;
        }
    }

    return cache[n - (size_t)1];
}
