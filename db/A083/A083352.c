/*
 * A083352  Least positive integer coefficients of power series A(x) such that
 *          the coefficients of A(x)^2 + A(x) - 1 consist entirely of squares.
 * https://oeis.org/A083352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 118, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083352
 *
 * Definition:
 *   a(0) = 1
 *   For n > 0:
 *     let S(n) = Σ_{i=1}^{n-1} a(i)·a(n-i)
 *     Let s be the smallest integer with s > √S(n) such that (s² - S(n)) is a multiple of 3.
 *     Then a(n) = (s² - S(n)) / 3
 *
 * The function below memoises the sequence in a static array and
 * computes missing terms on demand.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides: seq_t floor_sqrt(seq_t x) */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *coeff = NULL;      /* a[0..]               */
    static idx_t  coeff_cap = 0;     /* allocated size       */
    static idx_t  last_n   = 0;      /* largest computed idx */

    /* initialise cache on first call */
    if (coeff == NULL) {
        coeff_cap = 1;
        coeff = (seq_t *)calloc(coeff_cap, sizeof(seq_t));
        coeff[0] = (seq_t)1;   /* a(0) = 1 */
        last_n   = 0;
    }

    /* grow capacity if needed */
    if (n >= coeff_cap) {
        idx_t new_cap = coeff_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;          /* double until big enough */
        coeff = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
        for (idx_t i = coeff_cap; i < new_cap; ++i) coeff[i] = (seq_t)0;
        coeff_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        seq_t S = 0;                     /* Σ_{i=1}^{k-1} a(i)·a(k-i) */
        for (idx_t i = 1; i < k; ++i) {
            S += coeff[i] * coeff[k - i];
        }

        seq_t s = floor_sqrt(S) + (seq_t)1;        /* first integer > √S */
        while ( ( (s * s - S) % (seq_t)3 ) != (seq_t)0 ) {
            ++s;
        }

        coeff[k] = (s * s - S) / (seq_t)3;         /* a(k) = (s² - S)/3 */
    }

    last_n = n;
    return coeff[n];
}
