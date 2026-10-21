/*
 * A213557  Antidiagonal sums of the convolution array A213590.
 * https://oeis.org/A213557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 2*a(n-3) + 6*a(n-4) - 4*a(n-5) -
 *            a(n-6) + a(n-7) offset 1. Initial terms: a(1)=1, a(2)=6,
 *            a(3)=23, a(4)=70, a(5)=184, a(6)=438, a(7)=971
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 33, u64 n <= 77, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n==0 is outside the offset; return 0 (value unused) */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* number of terms stored (>=7 after init) */

    /* Ensure the cache is initialized with the first 7 terms */
    if (cache_cap == 0) {
        cache_cap = 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)6;   /* a(2) */
        cache[2] = (seq_t)23;  /* a(3) */
        cache[3] = (seq_t)70;  /* a(4) */
        cache[4] = (seq_t)184; /* a(5) */
        cache[5] = (seq_t)438; /* a(6) */
        cache[6] = (seq_t)971; /* a(7) */
    }

    /* grow cache if requested term not yet computed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;                /* need indices 0 .. n-1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                         /* allocation failure */
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            /* i corresponds to a(i+1) since cache index = n-1 */
            seq_t pos = cache[i-1] * (seq_t)5
                      + cache[i-3] * (seq_t)2
                      + cache[i-4] * (seq_t)6
                      + cache[i-7];

            seq_t neg = cache[i-2] * (seq_t)8
                      + cache[i-5] * (seq_t)4
                      + cache[i-6];           /* coefficient 1 */

            cache[i] = pos - neg;             /* positive ∈ seq_t */
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n - 1];
}
