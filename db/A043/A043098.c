/*
 * A043098  Numbers k such that 0 and 2 occur juxtaposed in the base-3
 *          representation of k but not of k-1.
 * https://oeis.org/A043098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_adj02
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: A043098 – numbers k such that 0 and 2 occur juxtaposed
   in the base‑3 representation of k but not of k-1.
   Implementation uses incremental search with memoization. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adj02.h"

static seq_t *cache = NULL;         /* cached values a(1), a(2), ... */
static idx_t  cache_len = 0;        /* number of cached terms       */
static idx_t  cache_cap = 0;        /* allocated capacity           */
static seq_t  next_candidate = (seq_t)1; /* next k to test              */

static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined; guard against underflow */
        return (seq_t)0;

    /* fill cache up to required index */
    while (cache_len < n) {
        seq_t k = next_candidate;
        while (true) {
            if (has_adj02(k) && !has_adj02(k - 1)) {
                /* store result */
                grow_cache(cache_len + 1);
                cache[cache_len++] = k;
                next_candidate = k + 1;
                break;
            }
            k++;
        }
    }
    return cache[n - 1];
}
