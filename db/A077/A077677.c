/*
 * A077677  Squarefree numbers beginning with 1.
 * https://oeis.org/A077677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 6753, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9923 (2026-11-01)
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth squarefree integer whose decimal representation starts with digit 1 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;
static seq_t  next_scan  = 1;

/* Generate terms up to the requested index n */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        /* Determine if next_scan begins with digit 1 */
        seq_t t   = next_scan;
        seq_t first = t;
        while (first >= 10) first /= 10;
        if (first == 1 && is_squarefree(next_scan))
        {
            /* Grow cache if necessary */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_scan;
        }
        ++next_scan;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* Undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
