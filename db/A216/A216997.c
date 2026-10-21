/*
 * A216997  Multiples of 8 that have a digit sum which is a multiple of 8.
 * https://oeis.org/A216997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A216997: multiples of 8 whose digit sum is a multiple of 8 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* cache for previously found terms */
static seq_t *cache = NULL;     /* array of found terms                    */
static size_t cache_len = 0;    /* number of terms stored in the cache      */
static size_t cache_cap = 0;    /* current capacity of the cache            */
static seq_t  last_test = 0;    /* last tested multiple of 8 */

/* Ensures that the cache can hold at least new_cap elements, reallocating if needed. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t at_least = cache_cap ? cache_cap * 2 : 2;
    if (at_least < new_cap) at_least = new_cap;

    /* Allocate new storage and copy existing elements */
    seq_t *new_ptr = (seq_t*)malloc(sizeof(seq_t) * at_least);
    for (size_t i = 0; i < cache_len; ++i)
        new_ptr[i] = cache[i];

    if (cache) free(cache);
    cache = new_ptr;
    cache_cap = at_least;
}

/* Returns the n‑th element of sequence A216997, with n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for offset 0 */

    /* If the value is already cached, return it. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Generate terms until we reach index n. */
    while (cache_len < (size_t)n) {
        /* Test the next multiple of 8. */
        seq_t val = last_test + (seq_t)8;
        last_test = val;

        /* Sum of digits. */
        seq_t ds = digit_sum(val);
        if ((ds % (seq_t)8) == 0) {
            /* Append to cache. */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
    }
    return cache[n - 1];
}
