/*
 * A009343  Expansion of e.g.f. log(1+sin(x)/exp(x)).
 * https://oeis.org/A009343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A009343: Expansion of e.g.f. log(1+sin(x)/exp(x)).
 * Computed via series inversion of the e.g.f. H(x)=1+sin(x)exp(-x):
 *   H(x)=∑H_n x^n/n! ,  H_0=1.
 *   Let B(x)=1/H(x)=∑b_n x^n/n!.
 *   Then F(x)=log(H(x)) satisfies F'(x)=H'(x)·B(x).
 *   Expanding gives
 *     d_n = ∑_{k=0}^n binom(n,k) H_{k+1} b_{n-k}   (coefficients of F'(x))
 *   The desired sequence is a_0=0 and for n≥1 : a_n = d_{n-1}.
 *
 * The routine caches all needed intermediate sequences and grows them on
 * demand, so arbitrary order of calls is supported.  All arithmetic is
 * performed in the integer type seq_t supplied by the build system.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *H_arr = NULL;   /* coefficients of 1 + sin(x)exp(-x)   */
static seq_t *b_arr = NULL;   /* coefficients of 1 / H(x)             */
static seq_t *d_arr = NULL;   /* coefficients of F'(x)               */
static seq_t *a_arr = NULL;   /* final sequence values                */
static size_t cache_len = 0;  /* current allocated length */

/* Ensure all tables are computed at least up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 2; /* we need indices up to n+1 for H */
    if (cache_len >= needed)
        return;

    /* Reallocate all arrays to the new size */
    H_arr = (seq_t *)realloc(H_arr, needed * sizeof(seq_t));
    b_arr = (seq_t *)realloc(b_arr, needed * sizeof(seq_t));
    d_arr = (seq_t *)realloc(d_arr, needed * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, needed * sizeof(seq_t));
    if (!H_arr || !b_arr || !d_arr || !a_arr)
        abort();

    /* Compute new entries */
    size_t i;
    /* H[0] is always 1 */
    if (cache_len == 0)
        H_arr[0] = (seq_t)1;

    /* Compute H[n] = ∑_{odd k} binom(n,k) (-1)^((k-1)/2) (-1)^(n-k) */
    for (i = (cache_len > 1 ? cache_len : 1); i < needed - 1; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;          /* binom(i,0) = 1 */
        size_t k;
        for (k = 1; k <= i; ++k) {
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k; /* binom(i,k) */
            if (k & 1) { /* odd k */
                size_t exp = ((k - 1) / 2) + (i - k);
                if ((exp & 1) == 0)
                    sum += binom;
                else
                    sum -= binom;
            }
        }
        H_arr[i] = sum;
    }
    /* Ensure H[n+1] exists for the last new n */
    if (needed > 1 && cache_len < needed) {
        seq_t sum = 0, binom = 1;
        size_t k;
        for (k = 1; k <= needed - 1; ++k) {
            binom = binom * (seq_t)(needed - 1 - k + 1) / (seq_t)k;
            if (k & 1) {
                size_t exp = ((k - 1) / 2) + ((needed - 1) - k);
                if ((exp & 1) == 0)
                    sum += binom;
                else
                    sum -= binom;
            }
        }
        H_arr[needed - 1] = sum;
    }

    /* Compute b[0] = 1 */
    if (cache_len == 0)
        b_arr[0] = (seq_t)1;
    for (i = (cache_len > 1 ? cache_len : 1); i < needed - 1; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;          /* binom(i,0) = 1 */
        size_t k;
        for (k = 1; k <= i; ++k) {
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k; /* binom(i,k) */
            sum += binom * H_arr[k] * b_arr[i - k];
        }
        b_arr[i] = -sum;           /* H_0 = 1 */
    }
    /* Ensure b[n] exists for the last new n */
    if (needed > 1 && cache_len < needed) {
        seq_t sum = 0, binom = 1;
        size_t k;
        for (k = 1; k <= needed - 1; ++k) {
            binom = binom * (seq_t)(needed - 1 - k + 1) / (seq_t)k;
            sum += binom * H_arr[k] * b_arr[(needed - 1) - k];
        }
        b_arr[needed - 1] = -sum;
    }

    /* Compute d[n] = ∑_{k=0}^n binom(n,k) H_{k+1} b_{n-k} */
    for (i = (cache_len > 0 ? cache_len : 0); i < needed - 1; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;          /* binom(i,0) = 1 */
        size_t k;
        for (k = 0; k <= i; ++k) {
            sum += binom * H_arr[k + 1] * b_arr[i - k];
            if (k < i) {
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        d_arr[i] = sum;
    }
    /* Ensure d[n] exists for the last new n */
    if (needed > 1 && cache_len < needed) {
        seq_t sum = 0, binom = 1;
        size_t k;
        for (k = 0; k <= needed - 2; ++k) {
            sum += binom * H_arr[k + 1] * b_arr[(needed - 2) - k];
            if (k < needed - 2) {
                binom = binom * (seq_t)((needed - 2) - k) / (seq_t)(k + 1);
            }
        }
        d_arr[needed - 2] = sum;
    }

    /* Compute a[n] = d[n-1] for n>=1, a[0]=0 */
    if (cache_len == 0)
        a_arr[0] = (seq_t)0;
    for (i = (cache_len > 1 ? cache_len : 1); i < needed; ++i) {
        a_arr[i] = d_arr[i - 1];
    }

    cache_len = needed - 1; /* last computed index is needed-1 */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_arr[(size_t)n];
}
