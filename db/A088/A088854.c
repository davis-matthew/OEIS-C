/*
 * A088854  a(n) = (2^(n-1))*(Integral_{x=0..1} (1+x^2)^n
 *          dx)/(Integral_{x=0..1} (1-x^2)^n dx).
 * https://oeis.org/A088854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088854.
   Recurrence:  n*a(n) = 2*(3*n-1)*a(n-1) - 4*(2*n-1)*a(n-2)
   Initial values: a(1) = 2, a(2) = 7
   (Offset = 1) */

#include <stdlib.h>         /* malloc, realloc */
#include <stdint.h>         /* uint64_t */

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i).  Index 0 unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  last_computed = 0;      /* highest n that has been filled */

    if (n == 0)                            /* offset is 1, but return 0 for n==0 */
        return (seq_t)0;

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n + 1;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;               /* out‑of‑memory: graceful failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first two terms if not already done */
    if (last_computed < 1) {
        cache[1] = (seq_t)2;
        last_computed = 1;
    }
    if (last_computed < 2 && n >= 2) {
        cache[2] = (seq_t)7;
        last_computed = 2;
    }

    /* Compute any new terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        if (sizeof(seq_t) <= 8) {          /* 64‑bit or smaller: use __int128 */
            unsigned __int128 t1 = (unsigned __int128)2 *
                                   (unsigned __int128)(3*i - 1) *
                                   (unsigned __int128)cache[i-1];
            unsigned __int128 t2 = (unsigned __int128)4 *
                                   (unsigned __int128)(2*i - 1) *
                                   (unsigned __int128)cache[i-2];
            unsigned __int128 num = t1 - t2;
            val = (seq_t)(num / (unsigned __int128)i);
        } else {                           /* arbitrary‑precision seq_t */
            seq_t t1 = (seq_t)2 *
                       (seq_t)(3*i - 1) *
                       cache[i-1];
            seq_t t2 = (seq_t)4 *
                       (seq_t)(2*i - 1) *
                       cache[i-2];
            val = (t1 - t2) / i;
        }
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
