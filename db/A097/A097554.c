/*
 * A097554  Number of positive words of length n in the monoid Br_7 of
 *          positive braids on 8 strands.
 * https://oeis.org/A097554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by the rational generating function  
   G(x) = (1 + x²)⁵ / (1 - 7x + 18x² - 25x³ + 24x⁴ - 15x⁵ + 6x⁶ - x⁷).  
   Consequently a(n) satisfies the linear recurrence

    a(n) = 7 a(n‑1) - 18 a(n‑2) + 25 a(n‑3) - 24 a(n‑4)
           + 15 a(n‑5) - 6 a(n‑6) + a(n‑7) + b(n)

   for n ≥ 7, where b(n) are the coefficients of the numerator  
   (1 + x²)⁵ :  b(0)=1, b(2)=5, b(4)=10, b(6)=10, b(8)=5, b(10)=1, otherwise b(n)=0.
   The first seven terms are provided below and are used as seeds. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dynamic cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t last_computed = 7;   /* index of first uncached element */

    /* base values for n < 7 */
    if (n < 7) {
        /* ensure cache contains the first seven terms */
        if (cache == NULL || cache_cap < 7) {
            size_t need = 7;
            cache = (seq_t *)malloc(need * sizeof(seq_t));
            if (!cache) return 0;                 /* allocation failure – unlikely for these limits */
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)7;
            cache[2] = (seq_t)36;
            cache[3] = (seq_t)151;
            cache[4] = (seq_t)570;
            cache[5] = (seq_t)2019;
            cache[6] = (seq_t)6893;
            cache_cap = need;
            last_computed = 7;
        }
        return cache[(size_t)n];
    }

    /* n >= 7 ----------------------------------------------------------- */
    size_t N = (size_t)n;

    /* init cache if necessary */
    if (cache == NULL) {
        size_t need = N + 1;
        if (need < 64) need = 64;
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)151;
        cache[4] = (seq_t)570;
        cache[5] = (seq_t)2019;
        cache[6] = (seq_t)6893;
        cache_cap = need;
        last_computed = 7;
    }
    else if (N + 1 > cache_cap) {
        cache_cap = N + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;
    }

    /* compute missing terms up to N */
    for (size_t i = last_computed; i <= N; ++i) {
        seq_t sum =
            (seq_t)7      * cache[i-1]   -
            (seq_t)18     * cache[i-2]   +
            (seq_t)25     * cache[i-3]   -
            (seq_t)24     * cache[i-4]   +
            (seq_t)15     * cache[i-5]   -
            (seq_t)6      * cache[i-6]   +
            cache[i-7];

        /* add numerator coefficient b(i) for i ≤ 10 */
        if (i <= 10) {
            if ((i & 1) == 0) {
                size_t k = i >> 1;                /* i = 2k */
                static const seq_t coeffs[6] = {1,5,10,10,5,1};
                if (k <= 5) sum += coeffs[k];
            }
        }

        cache[i] = sum;
    }
    last_computed = N + 1;
    return cache[N];
}
