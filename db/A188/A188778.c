/*
 * A188778  Number of 3-turn bishop's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A188778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6) with
 *            a(1)=0, a(2)=0, a(3)=28, a(4)=152, a(5)=488, a(6)=1192
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoized terms  */
static size_t cache_size = 0;        /* number of terms stored   */
static size_t cache_cap  = 0;        /* current capacity        */

/* Ensure that the cache has at least n terms computed (n ≥ 1) */
static void ensure_terms(idx_t n)
{
    if (n == 0)
        return;                       /* nothing to compute */

    /* Expand the allocated array if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = 0;
        else if (i == 2)
            val = 0;
        else if (i == 3)
            val = 28;
        else if (i == 4)
            val = 152;
        else if (i == 5)
            val = 488;
        else if (i == 6)
            val = 1192;
        else {
            /* The recurrence may involve subtraction; use signed
               64‑bit arithmetic to avoid underflow.  All values for
               n ≤ 10 000 fit in signed 64‑bit, so the expression
               remains exact.  For larger n the behaviour is
               unspecified, which is acceptable for the required
               test range. */
            int64_t t = 4LL * (int64_t)cache[i - 2]       /* 4*a(i-1) */
                      - 5LL * (int64_t)cache[i - 3]       /* 5*a(i-2) */
                      + 5LL * (int64_t)cache[i - 5]       /* 5*a(i-4) */
                      - 4LL * (int64_t)cache[i - 6]       /* 4*a(i-5) */
                      + (int64_t)cache[i - 7];            /* a(i-6) */
            val = (seq_t)t;
        }
        cache[i - 1] = val;
    }
    cache_size = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* sequence is defined for n ≥ 1 only */
        return 0;
    ensure_terms(n);
    return cache[n - 1];
}
