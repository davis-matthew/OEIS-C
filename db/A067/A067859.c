/*
 * A067859  a(1) = 1; a(n+1) = floor(sqrt(Sum_{k=1..n} a(k)^2)).
 * https://oeis.org/A067859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; a(n+1) = floor(sqrt(Sum_{k=1..n} a(k)^2))
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches results and works for arbitrary-precision seq_t as well as
   any unsigned integer type (8, 16, 32, 64, 128 bits). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *a_cache = NULL;   /* a_cache[i] holds a(i), 1‑based */
static idx_t cache_cap = 0;     /* current allocated capacity (>= 1) */
static idx_t max_cached = 0;    /* highest index that has been computed */
static seq_t sum_so_far = (seq_t)0; /* Sum_{k=1..max_cached} a(k)^2 */

/* Grow the cached array to cover index ‘n’ (inclusive). */
static void grow_to(idx_t n)
{
    if (n <= max_cached)
        return;   /* nothing to do */

    /* Ensure capacity. */
    size_t needed = (size_t)(n + 1);  /* +1 because we ignore index 0 */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t ai;
        if (i == 1)
            ai = (seq_t)1;           /* base case */
        else
            ai = floor_sqrt(sum_so_far); /* recurrence step */

        a_cache[i] = ai;
        sum_so_far += ai * ai;       /* update the running sum of squares */
    }

    max_cached = n;
}

seq_t a(idx_t n)
{
    /* OEIS only defines a(n) for n >= 1; we assume caller respects that. */
    if (n == 0)
        return (seq_t)0; /* undefined, but safe fallback */
    grow_to(n);
    return a_cache[n];
}
