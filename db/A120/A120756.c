/*
 * A120756  Expansion of g.f. x*(1 +82*x +366*x^2
 *          -189*x^3)/((1+3*x)*(1+9*x)*(1-21*x)).
 * https://oeis.org/A120756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*21^n + 9*(-9)^n + 24*(-3)^n + 18*[n=1]) / 54, n≥1 The
 *            sequence also satisfies the linear recurrence a(n) = 9*a(n-1) +
 *            225*a(n-2) + 567*a(n-3) for n≥5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised: subsequent calls may use previously
   computed values. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) (1‑based offset) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* helper: ensure cache is large enough to hold values up to idx */
    if (cache_cap < 4) {
        /* initialise base values for n = 1 .. 4 */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* fallback: no memory */
        cache[0] = (seq_t)1;            /* a(1) */
        cache[1] = (seq_t)91;           /* a(2) */
        cache[2] = (seq_t)1410;         /* a(3) */
        cache[3] = (seq_t)33543;        /* a(4) */
    }

    /* n may be very large but in tests it is < 10000 */
    size_t target = (size_t)n;
    if (target <= cache_cap) {
        return cache[target - 1];
    }

    /* Extend cache to hold values up to target */
    size_t new_cap = target;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* on allocation failure simply return 0 */
        return (seq_t)0;
    }
    cache = new_cache;
    /* Compute new values using the recurrence */
    for (size_t i = cache_cap + 1; i <= target; ++i) {
        /* i corresponds to a(i) */
        seq_t term1 = cache[i - 2] * (seq_t)9;   /* 9 * a(i-1) */
        seq_t term2 = cache[i - 3] * (seq_t)225; /* 225 * a(i-2) */
        seq_t term3 = cache[i - 4] * (seq_t)567; /* 567 * a(i-3) */
        cache[i - 1] = term1 + term2 + term3;
    }
    cache_cap = new_cap;
    return cache[target - 1];
}
