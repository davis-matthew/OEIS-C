/*
 * A031470  Numbers whose base-5 representation has the same number of 0's as
 *          4's.
 * https://oeis.org/A031470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base‑5 representation has the same number
 *            of 0’s as 4’s
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   equal_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_counts.h"

static seq_t *cache = NULL;          /* array of found members */
static idx_t  cache_size = 0;        /* number of cached members          */
static idx_t  cache_cap   = 0;        /* current capacity of cache         */
static seq_t  next_candidate = 1;     /* next number to test               */

/* ensure cache has room for one more element */
static void ensure_capacity(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);   /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

extern seq_t a(idx_t n)
{
    /* generate terms until we have n of them */
    while (cache_size < n) {
        if (equal_counts(next_candidate)) {
            ensure_capacity();
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
