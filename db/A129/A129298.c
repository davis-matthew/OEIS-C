/*
 * A129298  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+89)^2 = y^2.
 * https://oeis.org/A129298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 178 for n > 6 Initial values: a(1)=0,
 *            a(2)=51, a(3)=120, a(4)=267, a(5)=540, a(6)=931 This
 *            implementation uses memoization with a static cache
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 31, u64 n <= 69, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* constants for the first six terms */
    static const seq_t first6[6] = {0, 51, 120, 267, 540, 931};

    /* return constant directly for n <= 6 */
    if (n <= 6)
        return first6[n - 1];

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of elements stored so far */

    /* if we have not computed up to this n, grow the cache */
    if (n > cache_size) {
        /* determine new size */
        size_t new_size = (size_t)n;

        /* allocate or resize cache */
        seq_t *tmp = (cache == NULL)
                     ? (seq_t *)malloc(new_size * sizeof(seq_t))
                     : (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return zero (sequence values are non‑negative, so zero is distinct) */
            return (seq_t)0;
        }
        cache = tmp;

        /* initialize the first six entries if this is the first allocation */
        if (cache_size == 0) {
            for (idx_t i = 0; i < 6; ++i)
                cache[i] = first6[i];
        }

        /* compute new values using the recurrence */
        idx_t start = cache_size + 1; /* first index to compute */
        if (start < 7)
            start = 7;              /* recurrence applicable only for n > 6 */

        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = 6*a(i-3) - a(i-6) + 178
               array indices: a(k) stored at cache[k-1] */
            cache[i - 1] = 6 * cache[i - 4] - cache[i - 7] + (seq_t)178;
        }

        cache_size = n;
    }

    /* return the calculated (or cached) value */
    return cache[n - 1];
}
