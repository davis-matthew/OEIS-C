/*
 * A096563  McKay-Thompson series of class 25a for the Monster group.
 * https://oeis.org/A096563
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in 1 + x^-1 * Prod_{k>=1}
 *            (1-x^k)/(1-x^(25k))
 * Range:     i8 n <= 313, i16 n <= 925, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the McKay‑Thompson series of class 25a for the Monster group. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Handle index below the offset.  For n < -1 the value is undefined; */
/* return 0 for simplicity. */
    if (n < -1) {
        return (seq_t)0;
    }

    /* For n >= -1 we need coefficients of F(x) up to exponent n+1. */
    idx_t limit = n + 1;                /* number of coefficients needed in F */
    if (limit < 0) {                    /* protect against overflow when n = -1 + 1? */
        limit = 0;
    }
    size_t sz = (size_t)(limit + 1);    /* array size for f[0..limit] */

    /* Allocate an array of seq_t. Use C++ new if compiling as C++. */
#ifdef __cplusplus
    seq_t *f = new seq_t[sz];
    if (!f) return (seq_t)0;
#else
    seq_t *f = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!f) return (seq_t)0;
#endif

    /* Initialize array: zero all entries, then set f[0] = 1. */
    for (size_t i = 0; i < sz; ++i) {
        f[i] = (seq_t)0;
    }
    f[0] = (seq_t)1;

    /* Perform the infinite product expansion up to the required exponent. */
    for (idx_t k = 1; k <= limit; ++k) {
        /* (1 - x^k) part: subtract shifted array. */
        for (idx_t m = limit; m >= k; --m) {
            f[(size_t)m] -= f[(size_t)(m - k)];
        }

        /* 1/(1 - x^(25k)) part.  Only do it if 25*k <= limit. */
        idx_t d = 25 * k;
        if (d > limit) continue;
        for (idx_t m = d; m <= limit; ++m) {
            f[(size_t)m] += f[(size_t)(m - d)];
        }
    }

    /* Compute the required coefficient a(n). */
    seq_t result;
    if (n == -1) {            /* coefficient of x^-1 comes from F(0) */
        result = f[0];
    } else {
        /* a(n) = (n == 0 ? 1 : 0) + f[n+1] */
        const seq_t one_term = (n == 0) ? (seq_t)1 : (seq_t)0;
        result = one_term + f[(size_t)(n + 1)];
    }

    /* Free the array. */
#ifdef __cplusplus
    delete[] f;
#else
    free(f);
#endif

    return result;
}
