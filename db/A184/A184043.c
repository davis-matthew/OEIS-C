/*
 * A184043  1/9 the number of (n+1) X 5 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   get_cache_len, set_cache_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A184043
   Formula: a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4)  for n ≥ 5
   Initial values: a(1)=45, a(2)=51, a(3)=61, a(4)=81
   The function memoises the values for repeated calls. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_cache_len.h"
#include "../utils/set_cache_len.h"

/* Helper to grow the internal cache */
static void ensure_cache_capacity(size_t need)
{
    static size_t cap = 0;
    static seq_t *cache = NULL;
    if (need <= cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cap = new_cap;
    /* Return the static pointer so the main function can use it */
    (void)cap; (void)tmp;
}

/* The cache is maintained in a static inner block to keep it private. */
static seq_t *get_cache(void)
{
    static seq_t *cache = NULL;
    static size_t len = 0;
    return cache;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* value not defined, but return 0 to avoid UB */
    size_t need = (size_t)n;
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* Ensure cache capacity */
    if (need > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Fill missing values */
    for (size_t i = len; i < need; ++i) {
        if (i == 0)            cache[i] = 45;          /* a(1) */
        else if (i == 1)       cache[i] = 51;          /* a(2) */
        else if (i == 2)       cache[i] = 61;          /* a(3) */
        else if (i == 3)       cache[i] = 81;          /* a(4) */
        else {
            seq_t v1 = 3 * cache[i - 1];       /* 3*a(n-1) */
            seq_t v3 = 6 * cache[i - 3];       /* 6*a(n-3) */
            seq_t v4 = 4 * cache[i - 4];       /* 4*a(n-4) */
            cache[i] = v1 - v3 + v4;
        }
    }
    len = need;
    return cache[n - 1];
}
