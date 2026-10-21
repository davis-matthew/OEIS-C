/*
 * A205469  Number of (n+1)X3 0..2 arrays with every 2X2 subblock having the
 *          same number of equal edges, and new values 0..2 introduced in row
 *          major order.
 * https://oeis.org/A205469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) -85*a(n-2) +142*a(n-3) +159*a(n-4) -676*a(n-5)
 *            +465*a(n-6) +298*a(n-7) -506*a(n-8) +220*a(n-9) -32*a(n-10).
 *            Initial terms (a(1) … a(10)): 48, 263, 1548, 9654, 61978,
 *            405702, 2686876, 17926060, 120146998, 807610262
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of terms stored */
    static size_t cap = 0;   /* allocated capacity */

    /* initial 10 terms */
    static const seq_t init[10] = {
        48, 263, 1548, 9654, 61978,
        405702, 2686876, 17926060,
        120146998, 807610262
    };

    /* allocate or extend cache */
    if (cache == NULL) {
        cap = 32; /* moderate initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 10; ++i) cache[i] = init[i];
        len = 10;
    } else if (n > cap) {
        size_t new_cap = cap;
        while (new_cap < n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failure: return 0 */
        cache = new_cache;
        cap = new_cap;
    }

    /* return cached value if already computed */
    if (n <= len) return cache[n - 1];

    /* compute new values up to n */
    for (size_t i = len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 10) {
            val = init[i - 1];
        } else {
            /* compute recurrence using only additions/subtractions */
            seq_t pos = (seq_t)16 * cache[i - 2]
                      + (seq_t)142 * cache[i - 4]
                      + (seq_t)159 * cache[i - 5]
                      + (seq_t)465 * cache[i - 7]
                      + (seq_t)298 * cache[i - 8]
                      + (seq_t)220 * cache[i -10];

            seq_t neg = (seq_t)85  * cache[i - 3]
                      + (seq_t)676 * cache[i - 6]
                      + (seq_t)506 * cache[i - 9]
                      + (seq_t)32  * cache[i -11];

            /* pos >= neg by definition of the recurrence */
            val = pos - neg;
        }
        cache[i - 1] = val;
    }
    len = n;
    return cache[n - 1];
}
