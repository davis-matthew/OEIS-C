/*
 * A089115  Convoluted convolved Fibonacci numbers G_8^(r).
 * https://oeis.org/A089115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) + a(n-7) - 6*a(n-8) + 15*a(n-9) - 20*a(n-10) + 15*a(n-11)
 *            - 6*a(n-12) + a(n-13)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached sequence values */
    static size_t cache_len = 0;     /* number of values currently cached */
    static size_t cache_cap = 0;     /* allocated capacity of cache */

    /* sequence is defined for n >= 1; behaviour for n < 1 doesn't matter */
    if (n == 0) return 0;

    /* initialization on first call */
    if (cache_len == 0) {
        cache_cap = 32;                          /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;                    /* allocation failure */

        /* first 13 terms as provided by OEIS */
        cache[0]  = (seq_t)21;    /* a(1)  */
        cache[1]  = (seq_t)65;    /* a(2)  */
        cache[2]  = (seq_t)158;   /* a(3)  */
        cache[3]  = (seq_t)331;   /* a(4)  */
        cache[4]  = (seq_t)626;   /* a(5)  */
        cache[5]  = (seq_t)1098;  /* a(6)  */
        cache[6]  = (seq_t)1817;  /* a(7)  */
        cache[7]  = (seq_t)2871;  /* a(8)  */
        cache[8]  = (seq_t)4367;  /* a(9)  */
        cache[9]  = (seq_t)6435;  /* a(10) */
        cache[10] = (seq_t)9230;  /* a(11) */
        cache[11] = (seq_t)12935; /* a(12) */
        cache[12] = (seq_t)17764; /* a(13) */
        cache_len = 13;
    }

    /* If requested value is already cached, return it */
    if (n <= cache_len)
        return cache[n-1];

    /* Grow the cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n, using the recurrence */
    for (size_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum_pos =  (6 * cache[i-2]       /* a(i-1)   */
                        + (20 * cache[i-4])    /* a(i-3)   */
                        + (6  * cache[i-6])    /* a(i-5)   */
                        +      cache[i-8]      /* a(i-7)   */
                        + (15 * cache[i-10])   /* a(i-9)   */
                        + (15 * cache[i-12])   /* a(i-11)  */
                        +      cache[i-14]);   /* a(i-13)  */

        seq_t sum_neg =  (15 * cache[i-3]       /* a(i-2)   */
                        + (15 * cache[i-5])    /* a(i-4)   */
                        +      cache[i-7]      /* a(i-6)   */
                        + (6  * cache[i-9])    /* a(i-8)   */
                        + (20 * cache[i-11])   /* a(i-10)  */
                        + (6  * cache[i-13])); /* a(i-12)  */

        cache[i-1] = sum_pos - sum_neg;
    }
    cache_len = n;
    return cache[n-1];
}
