/*
 * A120657  Expansion of 2*x*(6 +59*x +257*x^2 - 294*x^3
 *          -128*x^4)/((1-x)*(1+x)*(1-2*x)*(1+3*x)*(1-4*x)).
 * https://oeis.org/A120657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0) = 0 a(1) = 12 a(2) = 154 a(3) = 1108 a(4) = 4106 a(5) =
 *            3*a(4)+11*a(3)-27*a(2)-10*a(1)-256 For n ≥ 6: a(n) = 3*a(n-1) +
 *            11*a(n-2) - 27*a(n-3) - 10*a(n-4) + 24*a(n-5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function is required to have external linkage */
seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[5] = {0, 12, 154, 1108, 4106};

    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_max = (idx_t)(-1); /* indicates empty */

    /* If caching is empty, initialise it for indices 0..4 */
    if (cached_max == (idx_t)(-1)) {
        /* Allocate storage for at least 5 indices (0..4) */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        cached_max = 4;
    }

    /* Ensure the cache covers the requested index n */
    if (n > cached_max) {
        idx_t new_max = n;
        /* Resize the cache */
        seq_t *tmp = (seq_t *)realloc(cache, (new_max + 1) * sizeof(seq_t));
        if (!tmp)          /* allocation failure – abort the program */
            abort();
        cache = tmp;

        /* Compute missing terms up to n */
        for (idx_t i = cached_max + 1; i <= new_max; ++i) {
            if (i <= 4) {
                cache[i] = init[i];
            } else if (i == 5) {
                /* a(5) = 3*a(4)+11*a(3)-27*a(2)-10*a(1)-256 */
                seq_t val = cache[4] * (seq_t)3;
                val += cache[3] * (seq_t)11;
                val -= cache[2] * (seq_t)27;
                val -= cache[1] * (seq_t)10;
                val -= (seq_t)256;
                cache[5] = val;
            } else {
                /* Recurrence for n ≥ 6 */
                seq_t val = cache[i-1] * (seq_t)3;
                val += cache[i-2] * (seq_t)11;
                val += cache[i-5] * (seq_t)24;
                val -= cache[i-3] * (seq_t)27;
                val -= cache[i-4] * (seq_t)10;
                cache[i] = val;
            }
        }

        cached_max = new_max;
    }

    return cache[n];
}
