/*
 * A014125  Bisection of A001400.
 * https://oeis.org/A014125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + n + 1, with a(0)=1, a(1)=3,
 *            a(2)=6, a(3)=11
 * Range:     u8 n <= 13, u16 n <= 102, u32 n <= 4257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases for n < 4 */
    if (n < 4) {
        switch (n) {
            case 0: return 1;
            case 1: return 3;
            case 2: return 6;
            case 3: return 11;
        }
    }

    /* Lazy cache for n >= 4 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size of cache   */
    static idx_t cache_max = 0;      /* highest index computed so far */

    /* Ensure cache can hold indices up to n. */
    if (cache_cap < (size_t)n + 1 || cache_cap < 4) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – terminate the program. */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_cap = new_cap;

        /* Initialize base values if not already done. */
        if (cache_max == 0) {
            cache[0] = 1;
            cache[1] = 3;
            cache[2] = 6;
            cache[3] = 11;
            cache_max = 3;
        }
    }

    /* Compute any missing values up to n. */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* a(i) = a(i-1) + a(i-3) - a(i-4) + i + 1 */
            seq_t val = cache[i - 1];
            val += cache[i - 3];
            val -= cache[i - 4];
            val += (seq_t)(i + 1);
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
