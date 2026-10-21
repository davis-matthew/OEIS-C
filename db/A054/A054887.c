/*
 * A054887  Layer counting sequence for hyperbolic tessellation by cuspidal
 *          triangles of angles (Pi/3, Pi/5, Pi/7).
 * https://oeis.org/A054887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A054887 – Layer counting sequence for hyperbolic tessellation  
   of cuspidal triangles of angles (π/3, π/5, π/7).  
   Generating function:  
        A(x) = x(1+x)(1−x³)(1−x⁵)(1−x⁷) /
               (1−2x+x⁴+x⁶−x¹⁰−x¹²+2x¹⁵−x¹⁶).  
   This gives the order‑16 linear recurrence (valid for n ≥ 18):  
        a(n) = 2 a(n−1) − a(n−4) − a(n−6) + a(n−10) + a(n−12)
               − 2 a(n−15) + a(n−16).  
   The first 17 terms are taken from the OEIS entry. */

#include <stdlib.h>   /* for malloc, realloc */

static const seq_t init_vals[17] = {
    (seq_t)1,     /* a(1) */
    (seq_t)3,     /* a(2) */
    (seq_t)6,     /* a(3) */
    (seq_t)11,    /* a(4) */
    (seq_t)20,    /* a(5) */
    (seq_t)36,    /* a(6) */
    (seq_t)64,    /* a(7) */
    (seq_t)113,   /* a(8) */
    (seq_t)200,   /* a(9) */
    (seq_t)354,   /* a(10) */
    (seq_t)626,   /* a(11) */
    (seq_t)1107,  /* a(12) */
    (seq_t)1958,  /* a(13) */
    (seq_t)3464,  /* a(14) */
    (seq_t)6128,  /* a(15) */
    (seq_t)10839, /* a(16) */
    (seq_t)19172  /* a(17) */
};

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* cache[0] = a(1), cache[1] = a(2), … */
    static idx_t  cache_len = 0;   /* number of terms stored */

    if (n < 1) return (seq_t)0;    /* offset is 1 – undefined for n==0 */

    /* Initialise cache with the first 17 terms on the first call */
    if (cache == NULL) {
        cache_len = 17;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;  /* allocation failure – return 0 */
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_len)
        return cache[n-1];

    /* Extend cache to size n (realloc) */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (!new_cache) return (seq_t)0;  /* allocation failure */
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Recurrence: a(n) = 2*a(n-1) - a(n-4) - a(n-6)
                        + a(n-10) + a(n-12) - 2*a(n-15) + a(n-16) */
        seq_t val = 2 * cache[i-2] - cache[i-5] - cache[i-7]
                     + cache[i-11] + cache[i-13]
                     - 2 * cache[i-16] + cache[i-17];
        cache[i-1] = val;
    }
    cache_len = n;
    return cache[n-1];
}
