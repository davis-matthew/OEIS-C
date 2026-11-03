/*
 * A160293  Numerator of Hermite(n, 11/30).
 * https://oeis.org/A160293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 11, a(n) = 11*a(n-1) - 450*(n-1)*a(n-2) for n ≥
 *            2 (OEIS A160293)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 411 (2026-11-03)
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

static seq_t *cache = NULL;        /* array holding computed values */
static size_t cache_cap = 0;       /* capacity of the array (bytes) */
static size_t cache_len = 0;       /* number of computed entries */

static void init_cache(void)
{
    if (cache == NULL) {
        /* allocate room for the first two terms */
        grow_seq(&cache, &cache_cap, (size_t)2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache_len = 2;
    }
}

static void ensure_n(idx_t n)
{
    init_cache();

    /* Make sure the array is big enough */
    if ((size_t)n + 1 > cache_cap) {
        grow_seq(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Compute any missing terms up to n */
    if ((size_t)n + 1 > cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = 11*a(i-1) - 450*(i-1)*a(i-2) */
            idx_t k   = (idx_t)(i - 1);            /* (i-1) fits in idx_t */
            cache[i] = (seq_t)11 * cache[i-1] -
                       (seq_t)(450 * k) * cache[i-2];
        }
        cache_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_n(n);
    return cache[(size_t)n];
}
