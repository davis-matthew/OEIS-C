/*
 * A045192  Numbers whose base-5 representation contains exactly one 0 and one
 *          3.
 * https://oeis.org/A045192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_one0_and_one3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-5 representation contains exactly one 0 and one 3 (search) */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t, etc. */
#include "../utils/has_one0_and_one3.h"

static seq_t *cache = NULL;    /* store generated terms */
static size_t  cache_len = 0;  /* number of terms stored */
static size_t  cache_cap = 0;  /* allocated capacity */
static seq_t   current   = 0;   /* last integer inspected */

/* create or increase cache space */
static inline void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap) ? cache_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* out of memory – abort silently (not desired in contest setting) */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* 1‑based indexing – assume n >= 1 */
    if (n == 0)
        return 0;

    /* generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t x = current + 1;
        current = x;
        if (has_one0_and_one3(x)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = x;
        }
    }
    return cache[n - 1];
}
