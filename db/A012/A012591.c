/*
 * A012591  tanh(arcsinh(x)*exp(x)) =
 *          x+2/2!*x^2-24/4!*x^4-140/5!*x^5-200/6!*x^6...
 * https://oeis.org/A012591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n!*coeff of x^n in tanh(arcsinh(x)*exp(x))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(n) is the integer sequence: 0,1,2,0,-24,-140,-200,5096,63616,...) */

#include <stddef.h>
#include <stdlib.h>

/* External integer types provided by the build system. */
extern idx_t  _floor_isqrt_u64(idx_t n);       /* unused, but aliased for safety */

/* The computation uses only integer arithmetic. */

seq_t a(idx_t n)
{
    /* Small constants for n = 0 and 1. */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    static idx_t  max_computed = (idx_t)0;      /* largest n already computed */
    static seq_t *c = NULL, *d = NULL, *v = NULL, *b = NULL, *f = NULL;
    static size_t cap = 0;                     /* current allocation size */

    /* If the needed index is already computed, return it. */
    if (n <= max_computed) {
        return c[(size_t)n];
    }

    /* Allocate enough space for all arrays up to n. */
    size_t size = (size_t)n + 1;

    /* Free old memory if we are recomputing. */
    free(c); free(d); free(v); free(b); free(f);

    c = (seq_t*)malloc(sizeof(seq_t) * size);
    d = (seq_t*)malloc(sizeof(seq_t) * size);
    v = (seq_t*)malloc(sizeof(seq_t) * size);
    b = (seq_t*)malloc(sizeof(seq_t) * size);
    /* f is needed for computing the b coefficients and has size floor(n/2)+1 */
    size_t msize = n / 2 + 1;
    f = (seq_t*)malloc(sizeof(seq_t) * msize);

    /* -----   1.  Compute the b‑series coefficients  ----- */
    f[0] = (seq_t)1;                       /* f[0] = 1  */
    for (idx_t m = 1; m <= n / 2; ++m) {
        seq_t t = (seq_t)(2 * m - 1);
        t = t * t;                         /* (2m-1)^2  */
        f[m] = f[m - 1] * t;               /* recurrence   f[m] = f[m‑1] * (2m‑1)^2 */
    }

    for (idx_t i = 0; i <= n; ++i) {
        idx_t m = i / 2;
        seq_t val = f[m];
        if (m & 1) val = -val;             /* sign = (-1)^m   */
        b[i] = val;                        /* b[i] = f[m] * (-1)^m */
    }

    /* -----   2.  Compute the v‑series:  v[n] = Σ_{i=0}^n binom(n,i) * b[i]   ----- */
    for (idx_t N = 0; N <= n; ++N) {
        seq_t sum = 0;
        seq_t bin = 1;                     /* binom(N,0) = 1   */
        for (idx_t i = 0; i <= N; ++i) {
            sum += bin * b[i];
            if (i < N) {
                bin = bin * (seq_t)(N - i);
                bin = bin / (seq_t)(i + 1);   /* exact division */
            }
        }
        v[N] = sum;
    }

    /* -----   3.  Initialize known values for a(n)  ----- */
    c[0] = (seq_t)0;
    c[1] = (seq_t)1;
    d[0] = (seq_t)1;
    d[1] = (seq_t)0;

    /* -----   4.  Compute a(n) for n >= 2  ----- */
    for (idx_t k = 1; k <= n - 1; ++k) {
        /* Compute d[k] for k >= 2  */
        if (k >= 2) {
            seq_t sumd = 0;
            seq_t bin = 1;                 /* binom(k,0) = 1   */
            for (idx_t i = 1; i < k; ++i) {
                /* bin ← binom(k,i)   */
                bin = bin * (seq_t)(k - i + 1);
                bin = bin / (seq_t)i;
                sumd += bin * c[i] * c[k - i];
            }
            d[k] = -sumd;
        }

        /* Compute c[k+1]  */
        seq_t sumc = 0;
        seq_t bin = 1;                     /* binom(k,0) = 1   */
        for (idx_t i = 0; i <= k; ++i) {
            sumc += bin * d[i] * v[k - i];
            if (i < k) {
                bin = bin * (seq_t)(k - i);
                bin = bin / (seq_t)(i + 1);
            }
        }
        c[k + 1] = sumc;
    }

    max_computed = n;
    return c[(size_t)n];
}
