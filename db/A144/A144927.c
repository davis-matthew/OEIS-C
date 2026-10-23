/*
 * A144927  Numbers n such that there exists an integer k with
 *          (n+7)^3-n^3=k^2.
 * https://oeis.org/A144927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144927
 *
 * Recurrence: a(1) = 7, a(2) = 1162
 *              a(n+2) = 110*a(n+1) – a(n) + 378   (n ≥ 1)
 *
 * The function returns the n‑th term (1‑based index).
 * It memoises computed terms, so any order of calls is efficient.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms (0‑based) */
static idx_t  cache_cap = 0;     /* number of allocated slots       */
static idx_t  cached_n  = 0;     /* how many terms have been stored  */

/* Ensure that terms 1..n are available in the cache. */
static void ensure_cache(idx_t n)
{
    /* Grow the cache if necessary. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 4;
        while (new_cap < n) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) return;                 /* allocation failed – keep old cache */
        cache = p;
        cache_cap = new_cap;
    }

    /* Initialise initial terms if they are not yet present. */
    if (cache_cap >= 1 && cached_n < 1) {
        cache[0] = (seq_t)7;   /* a(1) */
        cached_n = 1;
    }
    if (cache_cap >= 2 && cached_n < 2) {
        cache[1] = (seq_t)1162; /* a(2) */
        cached_n = 2;
    }

    /* Compute further terms up to n using the recurrence. */
    for (idx_t i = cached_n; i < n; ++i) {
        seq_t temp = cache[i-1] * (seq_t)110 + (seq_t)378;
        cache[i]   = temp - cache[i-2];
    }
    cached_n = n;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for n < 1 – return 0 */
    ensure_cache(n);
    return cache[n-1];
}
