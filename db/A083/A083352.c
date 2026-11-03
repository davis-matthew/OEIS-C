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
 * Validated: standard, pass, 106 terms through n = 274 (2026-11-03)
 * Helpers:   ipow_i32, mem
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
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *coeff = NULL;      /* a[0..]               */
    static idx_t  coeff_cap = 0;     /* allocated size       */
    static idx_t  last_n   = 0;      /* largest computed idx */

    /* initialise cache on first call */
    if (coeff == NULL) {
        coeff_cap = 1;
        coeff = mem_new(seq_t, coeff_cap);
        coeff[0] = (seq_t)1;   /* a(0) = 1 */
        last_n   = 0;
    }

    /* grow capacity if needed */
    if (n >= coeff_cap) {
        idx_t new_cap = coeff_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;          /* double until big enough */
        coeff = mem_resize(seq_t, coeff, new_cap);
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
