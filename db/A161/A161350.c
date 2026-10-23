/*
 * A161350  Numbers with property that the digital sum times the product of
 *          the digits is a square.
 * https://oeis.org/A161350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   digit_product, digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161350: numbers whose digit sum times the product of the digits is a perfect square. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

/* static cache for memoisation */
static seq_t *cache = NULL;       /* array of found terms           */
static size_t cache_cap = 0;      /* allocated capacity             */
static size_t cache_len = 0;      /* number of cached terms         */
static idx_t last_checked = 0;    /* last integer that was tested   */

/* expand the cache array if needed and add a new term to it */
static void add_to_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the nth term (n >= 1).  The function works for arbitrary call order. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n cached terms.  The loop
       examines successive integers until the required count is reached. */
    while (cache_len < n) {
        ++last_checked;

        seq_t cand = (seq_t)last_checked;   /* cast to seq_t (may wrap for large types) */

        seq_t sum   = digit_sum(cand);
        seq_t prod  = digit_product(cand);
        seq_t prod2 = sum * prod;           /* (digit_sum * digit_product) */

        seq_t sq = floor_sqrt(prod2);
        if (sq * sq == prod2) {
            add_to_cache(cand);
        }
    }
    return cache[n - 1];
}
