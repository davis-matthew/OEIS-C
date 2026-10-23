/*
 * A022517  Describe previous term from the right (method B - initial term is
 *          6).
 * https://oeis.org/A022517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   describe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 6,
   a(n) = describe(a(n-1)),
   where describe reads the previous term from the right, groups identical
   consecutive digits, and for each group writes the digit followed by the
   count of that digit. */
#include <stdlib.h>
#include "../utils/describe.h"

/* Memoization cache for computed terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored */

/* Ensure cache has capacity for at least 'needed' elements. */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;

    /* In C++, cast the void* result of realloc to seq_t*. */
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is zero‑based; we need terms 0..n inclusive. */
    size_t need = (size_t)n + 1;
    cache_grow(need);

    while (cache_len <= n) {
        if (cache_len == 0) {
            cache[0] = (seq_t)6;
        } else {
            cache[cache_len] = describe(cache[cache_len - 1]);
        }
        ++cache_len;
    }

    return cache[(size_t)n];
}
