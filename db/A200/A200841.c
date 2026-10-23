/*
 * A200841  Number of 0..n arrays x(0..5) of 6 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n > 7 Initial values: a(1)=64, a(2)=529,
 *            a(3)=2356, a(4)=7587, a(5)=19930, a(6)=45465, a(7)=93472
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache such that cache[i] = a(i+1) for i>=0 */
    static size_t cache_len = 0;
    static seq_t *cache = NULL;

    /* initial 7 terms */
    static const seq_t init[7] = {
        (seq_t)64,      /* a(1) */
        (seq_t)529,     /* a(2) */
        (seq_t)2356,    /* a(3) */
        (seq_t)7587,    /* a(4) */
        (seq_t)19930,   /* a(5) */
        (seq_t)45465,   /* a(6) */
        (seq_t)93472    /* a(7) */
    };

    if (n == 0)
        return (seq_t)0;            /* undefined for n=0 – harmless placeholder */

    /* convert index to size_t (safe for n up to about 10^4) */
    size_t target = (size_t)n;

    if (target <= cache_len)
        return cache[target - 1];

    /* grow cache to required size */
    size_t newlen = target;
    seq_t *newcache = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (!newcache)
        abort();                    /* memory allocation failed */
    cache = newcache;

    /* compute missing terms */
    for (size_t i = cache_len; i < newlen; ++i) {
        if (i < 7) {
            cache[i] = init[i];
        } else {
            seq_t val = (seq_t)0;
            /* a(n) = 7*a(n-1) -21*a(n-2) +35*a(n-3) -35*a(n-4)
                     +21*a(n-5) -7*a(n-6) + a(n-7)   (indices 0‑based) */
            val = val + (cache[i-1] * (seq_t)7);
            val = val - (cache[i-2] * (seq_t)21);
            val = val + (cache[i-3] * (seq_t)35);
            val = val - (cache[i-4] * (seq_t)35);
            val = val + (cache[i-5] * (seq_t)21);
            val = val - (cache[i-6] * (seq_t)7);
            val = val + (cache[i-7] * (seq_t)1);
            cache[i] = val;
        }
    }

    cache_len = newlen;
    return cache[target - 1];
}
