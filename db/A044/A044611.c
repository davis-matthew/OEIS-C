/*
 * A044611  Numbers n such that string 5,3 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has53
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044611: numbers n such that the base-8 representation of n
   contains the substring "53" while the base-8 representation of n+1
   does not. The implementation computes terms by searching integers
   in increasing order, caching results for repeated calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has53.h"

/* External linkage: returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0; /* last integer examined */

    if (cache == NULL) {
        /* initial allocation */
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        last_checked++;
        seq_t cand = (seq_t)last_checked;
        if (has53(cand) && !has53(cand + 1)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
}
