/*
 * A188493  a(n) = A188491(n-1) + A188495(n-1) + A188497(n-1).
 * https://oeis.org/A188493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A188491(n-1) + A188495(n-1) + A188497(n-1) The sequence
 *            satisfies the 14‑term linear recurrence a(n) = 2*a(n-1) +
 *            2*a(n-2) + 10*a(n-4) + 8*a(n-5) -2*a(n-6)-16*a(n-7)-10*a(n-8)-
 *            2*a(n-9) 4*a(n-10)+2*a(n-11) + 2*a(n-13)+ a(n-14) with initial
 *            values a(0)=0, a(1)=0, a(2)=2, a(3)=6, a(4)=14, a(...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* fixed initial terms (indices 0..13) */
    static const seq_t base[14] = {
        0, 0, 2, 6, 14, 31, 104, 344,
        1084, 3236, 9784, 29964, 92241, 282780
    };

    /* coefficients for a(n‑1) … a(n‑14) */
    static const int coeff[14] = {
        2, 2, 0, 10, 8, -2, -16, -10, -2, 4, 2, 0, 2, 1
    };

    if (n < 14) {
        /* return directly for the initial block */
        return base[n];
    }

    /* lazy static cache to store computed values */
    static seq_t *cache   = NULL;
    static idx_t cache_size   = 0;   /* allocated size */
    static idx_t computed_max = 0;   /* highest index that has been computed */

    /* first use – allocate cache and initialise with the base values */
    if (!cache) {
        size_t sz = 32;
        while (sz <= (size_t)n) sz <<= 1;
        cache = (seq_t*)realloc(NULL, sz * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – fall back to 0 */

        /* copy the 14 initial values into the cache */
        for (idx_t i = 0; i < 14; ++i) cache[i] = base[i];
        cache_size   = sz;
        computed_max = 13;
    } else if (n >= cache_size) {
        /* grow the cache if it is too small */
        size_t sz = cache_size;
        while (sz <= (size_t)n) sz <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sz * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – fall back to 0 */
        cache = tmp;
        cache_size = sz;
    }

    /* compute all missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 14; ++k) {
            int c = coeff[k];
            seq_t val = cache[i - 1 - k];
            if (c > 0)     pos += (seq_t)c * val;
            else if (c < 0) neg += (seq_t)(-c) * val;
            /* coeff == 0 contributes nothing */
        }
        cache[i] = pos - neg;
    }
    computed_max = n;
    return cache[n];
}
