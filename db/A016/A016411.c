/*
 * A016411  437th cyclotomic polynomial.
 * https://oeis.org/A016411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 396, i16 n <= 396, i32 n <= 396, i64 n <= 396,
 *            i128 n <= 396, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the cyclotomic polynomial Φ_437(x).
   The coefficients satisfy:
        a(0) =  1
        a(1) = -1
        a(n) = a(n-19) + a(n-23) - a(n-42)   (n ≥ 2, a(k)=0 for k<0).
   For n larger than the degree 396 of Φ_437 the coefficients are 0. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *coeff = NULL;   /* cached coefficients up to degree 396 */
static bool initialized = false;
static const idx_t DEGREE = 396;   /* (19-1)*(23-1) */

static void init_coeff(void)
{
    const uint64_t p = 19;
    const uint64_t q = 23;
    const uint64_t pq = p * q;

    coeff = (seq_t *)malloc((DEGREE + 1) * sizeof(seq_t));
    if (!coeff) return; /* allocation failure – leave uninitialized */

    /* a(0) = 1 */
    coeff[0] = 1;

    for (idx_t e = 1; e <= DEGREE; ++e) {
        seq_t rhs = 0;
        if (e == 1)
            rhs = -1;
        else if (e == 0)        /* only for completeness */
            rhs = 1;
        /* other RHS values (e==pq or e==pq+1) are > DEGREE, ignore */

        seq_t val = rhs;
        if (e >= p)
            val += coeff[e - p];
        if (e >= q)
            val += coeff[e - q];
        if (e >= p + q)
            val -= coeff[e - (p + q)];
        coeff[e] = val;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_coeff();

    if (!initialized)         /* fallback if allocation failed */
        return 0;

    if (n <= DEGREE)
        return coeff[n];
    else
        return 0;
}
