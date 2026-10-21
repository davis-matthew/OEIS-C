/*
 * A090412  A Chebyshev transform of 2^n.
 * https://oeis.org/A090412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 16, i32 n <= 34, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A090412
 * Formula: Recurrence
 *   a(0) = 1, a(1) = 2, a(2) = 3
 *   for n >= 3:
 *     2*(n+1)*a(n) - 3*(n+1)*a(n-1) + 8*(n-2)*a(n-2) + 12*(2-n)*a(n-3) = 0
 *
 * This is a third‑order linear recurrence with polynomial coefficients.
 * The implementation keeps a static cache of previously computed terms
 * so that calls in arbitrary order are O(max(n1,n2,...)) with
 * constant‑time extension.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached terms for memoisation */
static seq_t *seq_cache = NULL;
static size_t cache_len   = 0;   /* number of terms currently stored   */
static size_t cache_cap   = 0;   /* capacity of seq_cache              */

/* Ensure the cache has space for at least n+1 terms */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* Grow capacity if necessary */
    if (cache_cap == 0) {
        cache_cap = 4;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache)
            return;              /* (Out-of‑memory is unlikely in test harness) */
    }
    while (n >= cache_cap) {
        cache_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
        if (!seq_cache)
            return;
    }

    /* Initialise first three terms if cache is still empty */
    if (cache_len == 0) {
        seq_cache[0] = 1;
        seq_cache[1] = 2;
        seq_cache[2] = 3;
        cache_len   = 3;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t an_1 = seq_cache[i - 1];
        seq_t an_2 = seq_cache[i - 2];
        seq_t an_3 = seq_cache[i - 3];

        seq_t factor1 = (seq_t)(i + 1);           /* n+1   */
        seq_t factor2 = (seq_t)(i - 2);           /* n-2   */
        seq_t factor3 = (seq_t)(2) - (seq_t)i;    /* 2-n   */

        seq_t numerator =
            3 * factor1 * an_1
          - 8 * factor2 * an_2
          - 12 * factor3 * an_3;                 /* (2-n)*12 */

        seq_t denom = 2 * factor1;                 /* 2*(n+1) */

        /* Integer division – the recurrence guarantees divisibility */
        seq_t an = numerator / denom;

        seq_cache[cache_len++] = an;
    }
}

/* External‑linkage function requested by the build */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
