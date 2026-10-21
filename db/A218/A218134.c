/*
 * A218134  Norm of coefficients in the expansion of 1/(1 - 2*x - i*x^2),
 *          where i is the imaginary unit.
 * https://oeis.org/A218134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) + 4*a(n-3) - a(n-4) for n >= 4, with
 *            initial values a(0) = 1, a(1) = 4, a(2) = 17, a(3) = 80
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap  = 0;          /* number of terms currently stored */

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a safe sentinel */
            return (seq_t)0;
        }
        cache = tmp;

        /* Ensure the first four terms exist (overwriting is harmless) */
        if (new_cap > 0) cache[0] = (seq_t)1;
        if (new_cap > 1) cache[1] = (seq_t)4;
        if (new_cap > 2) cache[2] = (seq_t)17;
        if (new_cap > 3) cache[3] = (seq_t)80;

        /* Compute any new terms needed */
        size_t start = (cap > 3 ? cap : 4);
        for (size_t i = start; i <= (size_t)n; ++i) {
            cache[i] = (cache[i-1] * (seq_t)4)
                     + (cache[i-2] * (seq_t)2)
                     + (cache[i-3] * (seq_t)4)
                     - cache[i-4];
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
