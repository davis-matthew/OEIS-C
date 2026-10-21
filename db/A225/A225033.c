/*
 * A225033  Non-crossing, non-nesting, 7-colored set partitions.
 * https://oeis.org/A225033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 92*a(n-1) -3514*a(n-2) +72168*a(n-3) -860019*a(n-4)
 *            +5943768*a(n-5) -22055962*a(n-6) +33922100*a(n-7) -a(n-8)
 *            Initial values: a(0)=1, a(1)=8, a(2)=71, a(3)=715, a(4)=8212,
 *            a(5)=106205, a(6)=1514633, a(7)=23353828
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A225033.  */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored (indices 0 .. cache_len-1) */
    static size_t cache_cap = 0;  /* allocated capacity of the cache array */

    /* Coefficients of the recurrence.  The array order corresponds to
       a(n-1), a(n-2), ..., a(n-8).  */
    const int coeffs[8] = {
        92,      /* +92 * a(n-1)   */
       -3514,    /* -3514 * a(n-2) */
        72168,   /* +72168 * a(n-3)*/
       -860019,  /* -860019 * a(n-4) */
        5943768, /* +5943768 * a(n-5)*/
       -22055962, /* -22055962 * a(n-6) */
        33922100, /* +33922100 * a(n-7)*/
       -1      /* -1 * a(n-8) */
    };

    /* Initialise cache with the first 8 known terms on the first call */
    if (cache_len == 0) {
        /* Allocate a small initial buffer (at least 8) */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* first 8 terms of the sequence */
        cache[0] = 1;
        cache[1] = 8;
        cache[2] = 71;
        cache[3] = 715;
        cache[4] = 8212;
        cache[5] = 106205;
        cache[6] = 1514633;
        cache[7] = 23353828;
        cache_len = 8;
    }

    /* If the requested term is already in the cache, return it immediately */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache if necessary to hold up to term n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = 0;  /* sum of positive contributions */
        seq_t neg = 0;  /* sum of absolute values of negative contributions */
        for (int k = 1; k <= 8; ++k) {
            const seq_t val = cache[i - k];
            const int c = coeffs[k - 1];
            if (c > 0) {
                pos += (val * (seq_t)c);
            } else {
                neg += (val * (seq_t)(-c));
            }
        }
        cache[i] = pos - neg;
    }

    cache_len = n + 1;   /* we now have terms 0 .. n stored */
    return cache[n];
}
