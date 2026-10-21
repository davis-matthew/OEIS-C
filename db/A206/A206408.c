/*
 * A206408  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an equal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A206408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 78*a(n-2) + 86*a(n-3) + 17*a(n-4) - 41*a(n-5)
 *            + 10*a(n-6) Initial values: a(1)=423, a(2)=5199, a(3)=69585,
 *            a(4)=953865, a(5)=13148043, a(6)=181467207
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1 */

    static seq_t *cache = NULL;      /* 1‑based array of computed terms */
    static size_t cache_cap = 0;     /* allocated array capacity */
    static size_t cache_len = 0;     /* last index computed (at least 1) */

    /* Grow array if needed */
    size_t needed = (size_t)n + 1;   /* we use index 0 for dummy */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first six terms if not yet done */
    if (cache_len == 0) {
        cache[0] = 0;               /* dummy */
        cache[1] = (seq_t)423;
        cache[2] = (seq_t)5199;
        cache[3] = (seq_t)69585;
        cache[4] = (seq_t)953865;
        cache[5] = (seq_t)13148043;
        cache[6] = (seq_t)181467207;
        cache_len = 6;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t sum_plus  = 19 * cache[i-1]
                        + 86 * cache[i-3]
                        + 17 * cache[i-4]
                        + 10 * cache[i-6];
        seq_t sum_minus = 78 * cache[i-2]
                        + 41 * cache[i-5];
        cache[i] = sum_plus - sum_minus;
    }
    if (cache_len < (size_t)n)
        cache_len = (size_t)n;

    return cache[n];
}
