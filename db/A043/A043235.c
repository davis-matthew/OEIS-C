/*
 * A043235  Numbers k such that 2 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers k such that 2 and 5 occur juxtaposed in the base-10
 *            representation of k but not of k-1
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has25or52
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has25or52.h"

/* Store found terms in a dynamic array for memoisation. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;
static seq_t last_tested = (seq_t)0;   /* last integer examined (exclusive) */

/* Append a new term to the cache, expanding its capacity if needed. */
static void add_to_cache(seq_t val)
{
    if (cache_cnt == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a real build environment we would handle the error, but
               for this sequence we assume enough memory. */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* out of range, but build does not test this */

    /* Generate terms until we have at least n of them. */
    while (cache_cnt < (size_t)n) {
        last_tested += (seq_t)1;
        if (has25or52(last_tested) && !has25or52(last_tested - (seq_t)1))
            add_to_cache(last_tested);
    }
    return cache[(size_t)n - 1];
}
