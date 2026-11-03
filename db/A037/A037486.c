/*
 * A037486  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2.
 * https://oeis.org/A037486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-2) - 9*a(n-3); Initial values: a(1) = 1,
 *            a(2) = 11, a(3) = 100
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t mul9(seq_t x) {
    seq_t r = x;
    for (int i = 0; i < 8; ++i) r += x;   /* r = x * 9 */
    return r;
}

static seq_t *cache = NULL;          /* memoised values, cache[0] = a(1) */
static idx_t  cached_n = 0;          /* number of values currently in cache */
static size_t cache_cap = 0;         /* current capacity of cache */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range – behaviour doesn't matter */

    if (n <= cached_n)            /* already computed */
        return cache[n-1];

    /* Ensure sufficient capacity */
    if (cache_cap < (size_t)n)
        grow_seq(&cache, &cache_cap, (size_t)n);

    /* Compute new values up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = 1;
        else if (i == 2)
            val = 11;
        else if (i == 3)
            val = 100;
        else {
            seq_t t1 = mul9(cache[i-2]);   /* 9 * a(i-1) */
            seq_t t3 = mul9(cache[i-4]);   /* 9 * a(i-3) */
            val = t1 + cache[i-3] - t3;    /* a(i-2) + 9*a(i-1) - 9*a(i-3) */
        }
        cache[i-1] = val;
    }
    cached_n = n;
    return cache[n-1];
}
