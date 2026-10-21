/*
 * A227151  Nonnegative solutions of the Pell equation x^2 - 97*y^2 = +1.
 *          Solutions y = 6377352*a(n).
 * https://oeis.org/A227151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*62809633*a(n-1) - a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of elements stored (cache[0 .. len-1]) */
    static size_t cap   = 0;   /* capacity of cache array */
    const seq_t COEF = (seq_t)125619266;  /* 2*62809633 */

    /* If this is the first call, allocate and initialise the first two terms */
    if (len == 0) {
        size_t init_cap = 2;
        cache = (seq_t*)realloc(cache, init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory guard, though unlikely */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
        cap = init_cap;
    }

    /* Grow the cache if necessary and fill missing values using the recurrence */
    if (len <= (size_t)n) {
        size_t required = (size_t)n + 1;
        size_t new_cap = cap ? cap * 2 : 2;
        while (new_cap < required) new_cap <<= 1;

        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory guard */

        /* Compute the new terms */
        for (size_t i = len; i <= n; ++i) {
            cache[i] = COEF * cache[i-1] - cache[i-2];
        }

        len = required;
        cap = new_cap;
    }

    return cache[n];
}
