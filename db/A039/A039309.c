/*
 * A039309  Numbers whose base-7 representation has the same nonzero number of
 *          0's and 2's.
 * https://oeis.org/A039309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_zeros_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039309: Search for numbers whose base-7 representation
 * has the same nonzero number of 0's and 2's.  
 * The function builds the sequence incrementally and memoises it. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_zeros_twos.h"

/* Grow the cache array to at least 'need' elements.
 * Works both in C (using realloc) and C++ (using new/delete). */
static void grow_cache(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;

    size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
    while (new_cap < need)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
    /* copy existing data */
    for (size_t i = 0; i < *cap; ++i)
        new_arr[i] = (*arr)[i];
    delete[] *arr;
#else
    seq_t *new_arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
#endif
    *arr = new_arr;
    *cap = new_cap;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* behaviour below offset is irrelevant */

    /* Static memoisation state. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;
    static idx_t last_checked = 0;

    /* If we already have the answer cached, return it. */
    if (cache_size >= n)
        return cache[cache_size - 1 + 1 - (cache_size - n)];  /* cache[n-1] */

    /* Generate more terms until we reach the desired index. */
    while (cache_size < n) {
        ++last_checked;                 /* next candidate to test */

        seq_t candidate = (seq_t)last_checked;
        if (has_equal_zeros_twos(candidate)) {
            /* Ensure the cache has room. */
            grow_cache(&cache, &cache_cap, (size_t)(cache_size + 1));
            cache[cache_size++] = candidate;
        }
    }

    return cache[n - 1];
}
