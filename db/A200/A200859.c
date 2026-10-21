/*
 * A200859  a(n) = 2*a(n-1)+3*a(n-2)+5^n for n>1, a(0)=-2, a(1)=1.
 * https://oeis.org/A200859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 5^n ; a(0) = -2, a(1) = 1
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 12, i64 n <= 26, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Current value of 5^k. After initialisation this holds 5^1. */
    static seq_t pow5         = (seq_t)5;
    static idx_t  pow5_exp    = 1;   /* exponent of pow5 (currently 1) */

    /* One‑time initialisation of the cache and the first power of 5. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)(0);           /* allocation failure */

        cache[0] = (seq_t)(-2);
        cache[1] = (seq_t)1;
        cache_size = 2;                          /* indices 0 and 1 stored */
        /* pow5 is already 5^1, pow5_exp == 1 */
    }

    /* If the value is already cached, return it immediately. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Resize cache if required. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)(0);            /* reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Advance pow5 to 5^i. */
        pow5 = pow5 * (seq_t)5;
        pow5_exp++;   /* now pow5 has exponent i */

        /* Recurrence: a(i) = 2*a(i-1) + 3*a(i-2) + 5^i. */
        seq_t val = (seq_t)2 * cache[(size_t)(i - 1)]
                  + (seq_t)3 * cache[(size_t)(i - 2)]
                  + pow5;

        cache[(size_t)i] = val;
    }

    cache_size = needed;
    return cache[(size_t)n];
}
