/*
 * A039048  Numbers whose base-7 representation has the same number of 4's and
 *          6's.
 * https://oeis.org/A039048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   qualify
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039048:
 *  a(n) is the n‑th nonnegative integer whose base‑7 representation
 *  contains an equal number of digits 4 and 6;  a(1) = 0.
 *
 *  Implementation: a simple incremental search with memoisation.
 *  The function is re‑entrant and works for all supplied values
 *  of idx_t and seq_t.  It never caches the same value twice
 *  and starts testing from 1 after the initial value 0 has been cached.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/qualify.h"

/* static storage for the memoised sequence */
static seq_t *cache = NULL;
static idx_t   cache_len  = 0;  /* number of cached entries */
static idx_t   cache_cap  = 0;  /* current allocation capacity */
static seq_t   last_value = 1;  /* next integer to test after the cached part */

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise on the first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)0;               /* a(1) = 0 */
        cache_len = 1;
        last_value = 1;                     /* start testing from 1 */
    }

    /* return cached value if already computed */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* generate further terms until we reach the required index */
    while (cache_len < n) {
        if (qualify(last_value)) {
            if (cache_len == cache_cap) {   /* grow the cache if needed */
                cache_cap += cache_cap;    /* double */
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last_value;
        }
        ++last_value;                       /* next candidate */
    }

    return cache[n - 1];
}
