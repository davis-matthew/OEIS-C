/*
 * A178413  Numbers k such that the product of decimal digits of k divides
 *          k+1.
 * https://oeis.org/A178413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 199, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined by numbers k such that the product of decimal digits
 * of k divides k+1.  The function returns the n-th such number, with a(1)=1. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_product.h"

static seq_t *cache = NULL;         /* cached sequence values          */
static size_t cache_len = 0;        /* number of values currently cached */
static size_t cache_cap = 0;        /* capacity of the cache array      */
static seq_t next_candidate = 0;    /* next integer to test            */

/* Ensure the cache array has enough capacity to hold at least 'needed' items.
 * The variable name 'tmp' is used instead of 'new' to avoid conflicts with
 * the C++ keyword. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    seq_t *tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failure – terminate    */
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Assumes n >= 1 (OEIS offset = 1).  Handle calls in any order. */
    while (cache_len < (size_t)n) {
        if (next_candidate == 0)
            next_candidate = 1;     /* start from 1 on first use          */

        seq_t prod = digit_product(next_candidate);
        if (prod != 0) {
            seq_t mod = (next_candidate + 1) % prod;
            if (mod == 0) {
                if (cache_len >= cache_cap)
                    grow_cache();
                cache[cache_len++] = next_candidate;
            }
        }
        next_candidate = next_candidate + 1;
    }
    return cache[(size_t)n - 1];
}
