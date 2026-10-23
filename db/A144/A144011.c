/*
 * A144011  E.g.f. satisfies: A'(x) = 1/(1 - x*A(x))^2 with A(0)=1.
 * https://oeis.org/A144011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144011
   a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 10, a(4) = 72, a(5) = 704, ... */
#include <stdlib.h>
#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* static cache of values in the sequence */
    static idx_t cache_n   = 0;           /* last computed index            */
    static idx_t cache_cap = 0;           /* allocated size of arrays       */
    static seq_t *arr_a = NULL;          /* a[0..cache_cap-1] (sequence)   */
    static seq_t *arr_g = NULL;          /* g[0..cache_cap-1] (inverse coeffs) */

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) * 2 + 10;   /* exponential growth */
        if (cache_cap == 0) {
            arr_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
            arr_g = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            arr_a = (seq_t *)realloc(arr_a, new_cap * sizeof(seq_t));
            arr_g = (seq_t *)realloc(arr_g, new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    /* initialisation for the first call */
    if (cache_n == 0) {
        arr_a[0] = (seq_t)1;      /* a(0) = 1   */
        arr_g[0] = (seq_t)1;      /* g(0) = 1   */
    }

    /* compute values up to requested n */
    for (idx_t i = cache_n; i < n; ++i) {
        /* ---- compute h_i = coefficient of x^i in H = G^2 ---- */
        seq_t h = 0;
        seq_t binom = 1;          /* binom(i,0) */
        for (idx_t k = 0; k <= i; ++k) {
            seq_t prod = arr_g[k] * arr_g[i - k];
            prod = prod * binom;
            h += prod;
            if (k < i) {
                /* binom(i,k+1) = binom(i,k) * (i-k) / (k+1) */
                seq_t num = binom * ((seq_t)(i - k));
                seq_t den = ((seq_t)(k + 1));
                binom = num / den;
            }
        }
        arr_a[i + 1] = h;        /* a(i+1) = h_i */

        /* ---- compute g_{i+1} ---- */
        seq_t g_new = 0;
        /* recurrence: g_{n} = Σ_{k=1}^{n} C(n,k) * k * a_{k-1} * g_{n-k} */
        /* Here n = i+1 */
        seq_t bin = (seq_t)(i + 1);   /* binom(i+1,1) */
        for (idx_t k = 1; k <= i + 1; ++k) {
            seq_t term = bin * ((seq_t)k) * arr_a[k - 1] * arr_g[i + 1 - k];
            g_new += term;
            if (k < i + 1) {
                /* binom(i+1,k+1) = binom(i+1,k) * (i+1-k) / (k+1) */
                seq_t num = bin * ((seq_t)(i + 1 - k));
                seq_t den = ((seq_t)(k + 1));
                bin = num / den;
            }
        }
        arr_g[i + 1] = g_new;
    }
    cache_n = n;

    return arr_a[n];
}
