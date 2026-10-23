/*
 * A118409  Unsigned row sums of triangle A118407.
 * https://oeis.org/A118409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-4) - 3*a(n-5) + 4*a(n-8) - 4*a(n-9) (valid
 *            for n >= 10, initial values for n = 0..9 given below)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0 .. 9 */
    static const seq_t init[10] = {1, 1, 3, 5, 5, 7, 11, 17, 21, 27};

    /* Return for the first ten terms */
    if (n < 10)
        return init[n];

    /* Cached sequence values – expanded on demand */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current allocated size */
    static size_t comp  = 0;   /* number of values already computed (cache[0..comp-1]) */

    /* Lazy initialization */
    if (!cache) {
        cap = 16;                       /* start with some space */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        comp = 10;                       /* 0..9 are ready */
    }

    /* Ensure capacity for n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;    /* double until enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute values up to n if necessary */
    if (n >= comp) {
        for (idx_t i = comp; i <= n; ++i) {
            seq_t res = cache[i-1];                     /* a(n-1)   */
            res += cache[i-4] * (seq_t)3;               /* +3*a(n-4) */
            res -= cache[i-5] * (seq_t)3;               /* -3*a(n-5) */
            res += cache[i-8] * (seq_t)4;               /* +4*a(n-8) */
            res -= cache[i-9] * (seq_t)4;               /* -4*a(n-9) */
            cache[i] = res;
        }
        comp = n + 1;
    }

    return cache[n];
}
