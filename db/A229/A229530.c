/*
 * A229530  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly one
 *          mistake, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 57*a(n-2) + 56*a(n-3) - 16*a(n-4) (for n > 5)
 *            a(1) = 6, a(2) = 36, a(3) = 361, a(4) = 3064, a(5) = 24344
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 208 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0; /* largest n for which cache[n] is valid */

    /* Prepare cache capacity */
    ensure_cache(n, &cache, &cap);

    /* If already computed, return the cached value */
    if (n <= max_computed)
        return cache[n];

    /* Compute values up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1) {
            val = 6;
        } else if (k == 2) {
            val = 36;
        } else if (k == 3) {
            val = 361;
        } else if (k == 4) {
            val = 3064;
        } else if (k == 5) {
            val = 24344;
        } else {
            /* Recurrence: a(k) = 14*a(k-1) - 57*a(k-2) + 56*a(k-3) - 16*a(k-4) */
            seq_t t1 = cache[k-1] * 14;      /* 14*a(k-1) */
            seq_t t2 = cache[k-2] * 57;      /* 57*a(k-2) */
            seq_t t3 = cache[k-3] * 56;      /* 56*a(k-3) */
            seq_t t4 = cache[k-4] * 16;      /* 16*a(k-4) */

            /* Compute (14*a(k-1) + 56*a(k-3)) - (57*a(k-2) + 16*a(k-4)) */
            seq_t sum1 = t1 + t3;
            seq_t sum2 = t2 + t4;

            /* sum1 is always ≥ sum2 for n ≥ 6, so subtraction is safe. */
            val = sum1 - sum2;
        }
        cache[k] = val;
    }

    max_computed = n;
    return cache[n];
}
