/*
 * A138238  Alternating sum of the squares of the first n Jacobsthal numbers.
 * https://oeis.org/A138238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 9*a(n-2) + 2*a(n-3) – 8*a(n-4) for n ≥ 4 with
 *            initial values a(0)=0, a(1)=1, a(2)=0, a(3)=9
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for computed values (zero‑based index) */
static seq_t *cache = NULL;
static idx_t   cap   = 0;  /* allocated capacity */
static idx_t   len   = 0;  /* number of values stored (len==0 means not initialized) */

/* ensure the cache has room for indices up to 'need' (inclusive) */
static void ensure_cap(idx_t need)
{
    if (need < cap)
        return;
    idx_t new_cap = cap ? (cap << 1) : 4;          /* start with 4 elements */
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cap   = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise cache if first call */
    if (len == 0) {
        ensure_cap(3);                          /* indices 0..3 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)9;
        len = 4;
    }

    /* enlarge cache if needed */
    if (n >= len) {
        ensure_cap(n);
        for (idx_t i = len; i <= n; ++i) {
            /* compute a(i) using the recurrence */
            seq_t a1 = cache[(size_t)(i - 1)];
            seq_t a2 = cache[(size_t)(i - 2)];
            seq_t a3 = cache[(size_t)(i - 3)];
            seq_t a4 = cache[(size_t)(i - 4)];
            cache[(size_t)i] = (-2) * a1 + 9 * a2 + 2 * a3 - 8 * a4;
        }
        len = n + 1;
    }

    return cache[(size_t)n];
}
