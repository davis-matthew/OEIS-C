/*
 * A115332  E.g.f: exp(x/(1-5*x))/sqrt(1-25*x^2).
 * https://oeis.org/A115332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (5*n-4)*a(n-1) + 5*(n-1)*(5*n-4)*a(n-2)
 *               - 125*(n-1)*(n-2)^2*a(n-3);
 * Initial values: a(0)=1, a(1)=1, a(2)=36.
 * The function memoizes computed terms in a static array for
 * efficient repeated calls.
 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* array holding computed values   */
static size_t cache_len = 0;         /* one past the last stored index */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold indices 0..n */
    size_t old_len = cache_len;          /* length before possible resize */
    if (cache == NULL || old_len < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;

        /* Initialise the first few terms if they have not yet been set */
        if (old_len < 1 && (n + 1) > 0) cache[0] = (seq_t)1;
        if (old_len < 2 && (n + 1) > 1) cache[1] = (seq_t)1;
        if (old_len < 3 && (n + 1) > 2) cache[2] = (seq_t)36;

        cache_len = n + 1;
    }

    /* Compute any missing values from the current cache end up to n */
    idx_t start = old_len;
    if (start < 3) start = 3;            /* recurrence requires a(n-1), a(n-2), a(n-3) */

    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = ((seq_t)5 * (seq_t)i - (seq_t)4) * cache[i - 1];
        seq_t t2 = (seq_t)5 * (seq_t)(i - 1) *
                   ((seq_t)5 * (seq_t)i - (seq_t)4) * cache[i - 2];
        seq_t t3 = (seq_t)125 * (seq_t)(i - 1) *
                   ((seq_t)(i - 2) * (seq_t)(i - 2)) * cache[i - 3];
        cache[i] = t1 + t2 - t3;
    }

    return cache[n];
}
