/*
 * A044229  Numbers k such that string 5,2 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has52_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = the nth integer k such that the octal representation of k
   contains the substring "52", while the octal representation of k-1 does not.
   The implementation performs an incremental search and memoises found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has52_oct.h"

static seq_t *cache = NULL;      /* memoised sequence values          */
static size_t cache_len = 0;      /* number of values stored           */
static size_t cache_cap = 0;      /* allocated capacity of cache       */
static seq_t cur = (seq_t)0;      /* next integer to test              */
static bool prev_has = false;     /* whether previous integer had "52" */

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_len < (size_t)n) {
        bool h = has52_oct(cur);
        if (h && !prev_has) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = cur;
        }
        prev_has = h;
        cur++;
    }

    return cache[(size_t)(n - 1)];
}
