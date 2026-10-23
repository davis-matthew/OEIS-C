/*
 * A045265  Numbers whose base-5 representation contains no 2's and exactly
 *          three 3's.
 * https://oeis.org/A045265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-5 representation contains no 2's and exactly three 3's. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Auxiliary data shared by all calls */
static seq_t *g_cache = NULL;        /* cached terms */
static size_t g_cache_len = 0;       /* number of cached terms */
static size_t g_cache_cap = 0;       /* capacity of cached array */
static seq_t g_tested = 0;           /* last tested integer */

/* Check whether a number satisfies the definition */
static bool is_valid(seq_t x)
{
    seq_t y = x;
    int count3 = 0;
    const seq_t five = 5;

    while (y > 0) {
        int d = (int)(y % five);
        if (d == 2) return false;
        if (d == 3) count3++;
        y = y / five;
    }
    return count3 == 3;
}

/* Ensure the cache contains at least n terms */
static void ensure_terms(idx_t n)
{
    if (g_cache_len >= (size_t)n) return;

    /* Grow the cache if necessary */
    if (g_cache_cap < (size_t)n) {
        size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 32;
        while (new_cap < (size_t)n) new_cap <<= 1;

        seq_t *new_arr = alloc_seq_array(new_cap);
        if (g_cache) {
            for (size_t i = 0; i < g_cache_len; ++i)
                new_arr[i] = g_cache[i];
            free_seq_array(g_cache);
        }
        g_cache    = new_arr;
        g_cache_cap = new_cap;
    }

    /* Append new qualifying numbers until we have enough */
    while (g_cache_len < (size_t)n) {
        g_tested = g_tested + 1;
        if (is_valid(g_tested)) {
            g_cache[g_cache_len] = g_tested;
            g_cache_len++;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour below offset is irrelevant */
    ensure_terms(n);
    return g_cache[n - 1];
}
