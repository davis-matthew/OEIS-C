/*
 * A117720  Numbers for which the sum of the digits is the square root of the
 *          product of their digits.
 * https://oeis.org/A117720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 981 (2026-11-01)
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth non‑negative integer X such that
   digit_sum(X)^2 == digit_product(X).  The values are found by
   sequential search. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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

/* External linkage – required by the test harness */
seq_t a(idx_t n)
{
    /* Static cache of generated terms */
    static seq_t *cache = NULL;   /* array of cached terms            */
    static size_t cache_len = 0;  /* how many terms are currently cached */
    static size_t cache_cap = 0;  /* allocated capacity of the array    */
    static idx_t next_candidate = 0;   /* next integer to test             */

    /* Initialise the cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    }

    /* Generate terms until we have at least (n+1) entries */
    while (cache_len <= (size_t)n) {
        seq_t value = (seq_t)next_candidate;
        seq_t sum   = digit_sum(value);
        seq_t prod  = digit_product(value);

        if (sum * sum == prod) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp) {
                    cache = tmp;
                    cache_cap = new_cap;
                }
            }
            cache[cache_len++] = value;
        }
        next_candidate++;
    }

    return cache[(size_t)n];
}
