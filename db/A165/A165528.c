/*
 * A165528  Number of permutations of length n which avoid the patterns 1324
 *          and 4231.
 * https://oeis.org/A165528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 70*a(n-2) + 202*a(n-3) - 336*a(n-4) +
 *            320*a(n-5) - 160*a(n-6) + 32*a(n-7) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=22, a(5)=86, a(6)=336, a(7)=1282
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;      /* Array of computed terms */
    static size_t cache_cap = 0;      /* Current allocated capacity */
    static idx_t cache_len = 0;        /* Number of terms computed and stored */

    /* Helper to ensure the cache has at least (n+1) slots and compute new terms if needed */
    if (n >= cache_len) {
        /* Ensure capacity */
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap > 0 ? cache_cap : 8;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed; fall back to minimal operation */
                /* In this unlikely event, we will recompute all terms from scratch each call. */
                cache = tmp; /* might be NULL, but in that case the loop below will allocate*/
                cache_cap = new_cap;
            } else {
                cache = tmp;
                cache_cap = new_cap;
            }
        }

        /* If this is the first call, initialize the first 8 known terms */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)2;
            cache[3] = (seq_t)6;
            cache[4] = (seq_t)22;
            cache[5] = (seq_t)86;
            cache[6] = (seq_t)336;
            cache[7] = (seq_t)1282;
            cache_len = 8;
        }

        /* Compute any new terms up to n using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 8) continue; /* already known */

            /* Positive contributions: 13, 202, 320, 32 */
            seq_t pos = 13 * cache[i-1] + 202 * cache[i-3]
                      + 320 * cache[i-5] + 32 * cache[i-7];

            /* Negative contributions: 70, 336, 160 */
            seq_t neg = 70 * cache[i-2] + 336 * cache[i-4]
                      + 160 * cache[i-6];

            cache[i] = pos - neg;   /* pos > neg by definition */
        }
        cache_len = n + 1;
        return cache[n];
    }

    /* n is within the cached range */
    return cache[n];
}
