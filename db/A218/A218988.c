/*
 * A218988  Power floor sequence of 2+sqrt(8).
 * https://oeis.org/A218988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218988: Power floor sequence of 2+sqrt(8)
 *
 * Recurrence:  a(0) = 4, a(1) = 19,
 *              a(n) = 4*a(n-1) + 4*a(n-2) - 1    for n >= 2
 *
 * The function memoizes computed values so that calls in any order
 * are efficient.  All operations are performed in the integer type
 * seq_t provided by the build, so the result is exact whenever it
 * fits in seq_t.
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoized terms       */
static size_t cache_cap = 0;  /* allocated capacity   */
static size_t cache_len = 0;  /* valid terms: 0 .. cache_len-1 */

/* Ensure we have memory for indices up to `needed` (inclusive). */
static void ensure_capacity(idx_t needed)
{
    /* needed+1 because we store a(0) .. a(needed) */
    size_t required = (size_t)needed + 1;
    if (required <= cache_cap) return;

    /* grow capacity to a power of two >= required */
    size_t new_cap = 1;
    while (new_cap < required) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – terminate */
        exit(EXIT_FAILURE);
    }
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Public function: compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)19;

    ensure_capacity(n);

    /* initialize base terms if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)19;
        cache_len = 2;
    }

    if ((size_t)n < cache_len) return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 4*a(i-1) + 4*a(i-2) - 1 */
        cache[i] = cache[i-1] * (seq_t)4
                 + cache[i-2] * (seq_t)4
                 - (seq_t)1;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
