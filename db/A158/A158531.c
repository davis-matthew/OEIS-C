/*
 * A158531  a(n) = Hermite(n,8).
 * https://oeis.org/A158531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n,8) Recurrence: a(0)=1, a(1)=16, a(n) =
 *            16*a(n-1)-(2*n-2)*a(n-2) (n>=2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 16, i128 n <= 34,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 720 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t last_computed = 1;  /* last index that has been filled */

seq_t a(idx_t n)
{
    /* ensure base values are available */
    if (!cache) {
        cache_cap = 2;
        cache = seq_alloc_array(cache_cap);
        cache[0] = 1;
        cache[1] = 16;
        last_computed = 1;
    }

    if (n < cache_cap && n <= last_computed)
        return cache[n];

    if (n >= cache_cap) {
        /* grow the cache to hold up to n */
        idx_t new_cap = n + 1;
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free_seq(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term1 = 16 * cache[i - 1];
        seq_t coeff = (seq_t)((idx_t)2 * i - 2);
        seq_t term2 = coeff * cache[i - 2];
        cache[i] = term1 - term2;
    }

    last_computed = n;
    return cache[n];
}
