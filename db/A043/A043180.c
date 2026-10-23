/*
 * A043180  Numbers k such that 0 and 1 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adj_zero_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k where the base‑9 representation of k contains adjacent digits 0 and 1,
   but the base‑9 representation of k‑1 does not.  Computed by incremental search
   with memoization. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_adj_zero_one.h"

/* Cached results of the sequence. */
static seq_t *cache       = NULL;   /* array of found terms   */
static size_t cache_len   = 0;      /* number of terms stored */
static size_t cache_cap   = 0;      /* current capacity of the array   */
static seq_t next_k       = 0;      /* last integer examined, starts from 0 */

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller. */
    if (n == 0) return 0;

    /* Lazily initialise the cache. */
    if (cache == NULL) {
        cache_cap = 4;                       /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure */
    }

    /* Generate terms until the cache contains at least n elements. */
    while (cache_len < (size_t)n) {
        ++next_k;
        seq_t cur  = next_k;
        seq_t prev = cur - 1;                 /* safe: next_k starts at 1 */

        bool cur_has  = has_adj_zero_one(cur);
        bool prev_has = has_adj_zero_one(prev);

        if (cur_has && !prev_has) {
            /* Append cur to the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return 0;   /* allocation failure */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
    }

    return cache[n - 1];
}
