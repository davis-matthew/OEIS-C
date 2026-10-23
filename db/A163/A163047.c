/*
 * A163047  Number of n X 3 binary arrays with all 1s connected, and all
 *          corners 1.
 * https://oeis.org/A163047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) + 6*a(n-3) + a(n-4) - 7*a(n-5) +
 *            a(n-6) for n>6 with initial values: a(1)=1, a(2)=3, a(3)=12,
 *            a(4)=58, a(5)=293, a(6)=1481
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle offset.  Values for n < 1 are undefined; return 0. */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoization.  cache[i] holds a(i), 1-indexed.  A zero value
       indicates "not yet computed", which is safe because the sequence is positive. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* allocated size, number of elements (including index 0) */

    /* Ensure cache holds indices up to n. */
    if (cache_len <= n) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;
        /* Initialize new slots to 0 (sentinel). */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* Return cached value if available. */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Compute base cases explicitly. */
    if (n == 1) { cache[1] = (seq_t)1; return cache[1]; }
    if (n == 2) { cache[2] = (seq_t)3; return cache[2]; }
    if (n == 3) { cache[3] = (seq_t)12; return cache[3]; }
    if (n == 4) { cache[4] = (seq_t)58; return cache[4]; }
    if (n == 5) { cache[5] = (seq_t)293; return cache[5]; }
    if (n == 6) { cache[6] = (seq_t)1481; return cache[6]; }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            seq_t t6 = cache[i-6];
            seq_t val = (seq_t)7 * t1
                        - (seq_t)11 * t2
                        + (seq_t)6 * t3
                        + t4
                        - (seq_t)7 * t5
                        + t6;
            cache[i] = val;
        }
    }

    return cache[n];
}
