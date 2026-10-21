/*
 * A211332  Number of (n+1)X(n+1) -5..5 symmetric matrices with every 2X2
 *          subblock having sum zero and two or three distinct values.
 * https://oeis.org/A211332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)+8*a(n-2)-75*a(n-3)+16*a(n-4)+467*a(n-5)-412*a(n-6) *
 *            -1551*a(n-7)+2039*a(n-8)+2909*a(n-9)-5118*a(n-10)-2897*a(n-11) *
 *            +7385*a(n-12)+976*a(n-13)-6197*a(n-14)+720*a(n-15)+2888*a(n-16)
 *            * -760*a(n-17)-674*a(n-18)+236*a(n-19)+60*a(n-20)-24*a(n-21) *
 *            Initial values: a(1)=60, a(2)=144, …...
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 57, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Lightweight version of ensure_capacity if the build does not provide it. */
#if !defined(ensure_capacity)
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (needed > *cap) {
        size_t new_cap = (needed > (*cap << 1)) ? needed : (*cap << 1);
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}
#endif

seq_t a(idx_t n) {
    /* Recurrence coefficients (a(n-1) … a(n-21)) as int32_t to preserve signs. */
    static const int32_t coeff[21] = {
        5, 8, -75, 16, 467, -412, -1551, 2039, 2909, -5118,
        -2897, 7385, 976, -6197, 720, 2888, -760, -674,
        236, 60, -24
    };
    /* Initial terms a(1) … a(21). */
    static const seq_t init[21] = {
        60, 144, 312, 630, 1266, 2474, 4864, 9466, 18544, 36252,
        71260, 140314, 277456, 550506, 1095678, 2189478, 4384860,
        8817146, 17756308, 35895256, 72635652
    };

    /* Cache of previously computed terms: cache[i] == a(i+1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored so far */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n == 0) return 0;                 /* undefined for n<1, but return 0 safely */
    size_t target = (size_t)n;            /* number of terms we need in the cache */

    /* Ensure the cache has room for the required index. */
    if (target > cache_len) {
        ensure_capacity(target, &cache_cap, (void **)&cache, sizeof(seq_t));

        /* Load the known initial terms into the cache if they are not yet present. */
        for (size_t i = cache_len; i < 21 && i < target; ++i)
            cache[i] = init[i];

        /* Compute any missing terms using the recurrence. */
        for (size_t idx = cache_len; idx < target; ++idx) {
            /* For idx < 21 we already have the value from init. */
            if (idx < 21) continue;

            seq_t pos = 0, neg = 0;   /* accumulate positive and negative contributions */
            for (int k = 0; k < 21; ++k) {
                seq_t term  = cache[idx - 1 - k];   /* a((idx+1)-(k+1)) = a(idx-k) */
                int32_t c   = coeff[k];
                uint32_t mag = (c >= 0) ? (uint32_t)c : (uint32_t)(-c);
                seq_t prod   = (seq_t)mag * term;
                if (c >= 0) pos += prod; else neg += prod;
            }
            cache[idx] = pos - neg;
        }
        cache_len = target;   /* update stored length */
    }

    return cache[n - 1];      /* return a(n) */
}
