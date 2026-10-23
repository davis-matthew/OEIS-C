/*
 * A165874  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter group
 *            on 14 generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 *            The sequence is defined by the rational generating function G(t)
 *            = (t^10 + 2 t^9 + 2 t^8 + 2 t^7 + 2 t^6 + 2 t^5 + 2 t^4 + 2 t^3
 *            + 2 t^2 + 2 t + 1) / (78 t^10 - 12 t^9 -...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------------- */

#include <stdlib.h>

/* The function must have external linkage */
seq_t a(idx_t n)
{
    /* Constant coefficients of the numerator (p[n]) for n=0..10 */
    static const seq_t p_vals[11] = {
        (seq_t)1,  /* p[0] */
        (seq_t)2,  /* p[1] */
        (seq_t)2,  /* p[2] */
        (seq_t)2,  /* p[3] */
        (seq_t)2,  /* p[4] */
        (seq_t)2,  /* p[5] */
        (seq_t)2,  /* p[6] */
        (seq_t)2,  /* p[7] */
        (seq_t)2,  /* p[8] */
        (seq_t)2,  /* p[9] */
        (seq_t)1   /* p[10] */
    };

    /* Cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;    /* allocated size */
    static idx_t cache_max = 0;    /* maximum index computed */

    /* Ensure cache is large enough and initialized */
    if (cache == NULL) {
        /* Allocate at least 16 slots to avoid frequent reallocs */
        cache_cap = (n + 1 > 16) ? n + 1 : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) = 1 (from G(t)) */
        cache_max = 0;
    } else if ((idx_t)(n + 1) > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 9) {
            /* a[i] = p[i] + 12 * (a[i-1] + ... + a[0]) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += cache[i - k];
            }
            cache[i] = p_vals[i] + sum * (seq_t)12;
        } else if (i == 10) {
            /* a[10] = 1 + 12 * (a[9] + ... + a[1]) - 78 * a[0] */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 9; ++k) {
                sum += cache[i - k];
            }
            seq_t term = sum * (seq_t)12;
            seq_t sub  = cache[0] * (seq_t)78;
            cache[i] = (seq_t)1 + term - sub;
        } else {
            /* For i > 10: a[i] = 12 * (a[i-1] + ... + a[i-9]) - 78 * a[i-10] */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 9; ++k) {
                sum += cache[i - k];
            }
            seq_t term = sum * (seq_t)12;
            seq_t sub  = cache[i - 10] * (seq_t)78;
            cache[i] = term - sub;
        }
    }

    cache_max = n;
    return cache[n];
}
