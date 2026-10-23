/*
 * A016352  190th cyclotomic polynomial.
 * https://oeis.org/A016352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 190th cyclotomic polynomial
 *            Φ_190(x)
 * Range:     i8 n <= 72, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   poly_div128, poly_mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The coefficients of Φ_190(x) are all in the set {–1,0,1}.  This routine
    computes Φ_190(x) once, stores the coefficients in a static array,
    and returns the requested coefficient.
    It uses  __int128  for intermediate arithmetic so that the routine
    works correctly even when the seq_t type is small (e.g. int8_t).  */

/* Required headers */
#include <stddef.h>   /* size_t, idx_t */
#include <stdlib.h>   /* malloc, free, abort */
#include <stdbool.h>  /* bool */
#include <string.h>   /* memset */
#include "../utils/poly_div128.h"
#include "../utils/poly_mul128.h"

/* static storage for the cyclotomic polynomials Φ_n(x), 1 ≤ n ≤ 190 */
#define NMAX 190
static seq_t *poly[NMAX + 1];          /* poly[n][i] = coefficient of x^i in Φ_n(x) */
static size_t deg_poly[NMAX + 1];      /* degree of Φ_n(x) */
static bool initialized = false;

/* list of all divisors of a number ≤ NMAX (excluding the number itself) */
static void divisors(idx_t n, idx_t *dv, size_t *cnt)
{
    *cnt = 0;
    for (idx_t d = 1; d < n; ++d) {
        if (n % d == 0) {
            dv[(*cnt)++] = d;
        }
    }
}

/* initialise all Φ_n(x) for 1 ≤ n ≤ 190   */
static void init_polynomials(void)
{
    /* allocate storage for each polynomial */
    for (idx_t n = 1; n <= NMAX; ++n) {
        poly[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!poly[n]) abort();
        deg_poly[n] = 0;
    }

    /* compute Φ_d for increasing d */
    for (idx_t n = 1; n <= NMAX; ++n) {
        /* compute product of Φ_d for proper divisors d of n   */
        __int128 prod[256];
        for (size_t i = 0; i < 256; ++i) prod[i] = 0;
        prod[0] = 1;
        size_t degProd = 0;

        /* collect divisors */
        idx_t dvs[64];
        size_t dcnt;
        divisors(n, dvs, &dcnt);
        for (size_t k = 0; k < dcnt; ++k) {
            idx_t d = dvs[k];
            /* convert stored Φ_d into __int128 array */
            __int128 mul[256];
            for (size_t i = 0; i < 256; ++i) mul[i] = 0;
            for (size_t i = 0; i <= deg_poly[d]; ++i)
                mul[i] = ( __int128 )poly[d][i];
            size_t degMul = deg_poly[d];
            /* multiply */
            __int128 newProd[256];
            size_t newDeg;
            poly_mul128(prod, degProd, mul, degMul, newProd, &newDeg);
            /* copy back */
            for (size_t i = 0; i <= newDeg; ++i) prod[i] = newProd[i];
            degProd = newDeg;
        }

        /* dividend polynomial x^n - 1   */
        __int128 dividend[256];
        for (size_t i = 0; i < 256; ++i) dividend[i] = 0;
        dividend[n] = 1;
        dividend[0] = -1;
        size_t degDiv = n;

        /* divide to obtain Φ_n  */
        __int128 quotient[256];
        size_t degQ;
        poly_div128(dividend, degDiv, prod, degProd, quotient, &degQ);

        /* store results in seq_t array */
        for (size_t i = 0; i <= degQ; ++i)
            poly[n][i] = (seq_t)quotient[i];
        deg_poly[n] = degQ;
    }

    initialized = true;
}

/* public entry point: coefficient of x^n in Φ_190(x) */
seq_t a(idx_t n)
{
    if (!initialized) init_polynomials();

    const size_t target = 190;
    if (n <= deg_poly[target])
        return poly[target][(size_t)n];
    return (seq_t)0;
}
