/*
 * A215008  a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3), a(0)=0, a(1)=1, a(2)=5.
 * https://oeis.org/A215008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=5, a(n)=7*a(n-1)-14*a(n-2)+7*a(n-3) for
 *            n>=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n<3 we can return directly. */
    if (n < 3) {
        if (n == 0) return (seq_t)0;
        if (n == 1) return (seq_t)1;
        return (seq_t)5;           /* n==2 */
    }

    /* Lazy initialization and memoisation. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 2;   /* values up to cache[2] are ready */

    /* Ensure the cache holds at least indices 0..2. */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
    } else if (cap < 3) {            /* In case a very small allocation happened */
        cap = 3;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing values up to n. */
    if ((idx_t)n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[(size_t)i] =
                (seq_t)7 * cache[(size_t)(i - 1)]
                - (seq_t)14 * cache[(size_t)(i - 2)]
                + (seq_t)7  * cache[(size_t)(i - 3)];
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
