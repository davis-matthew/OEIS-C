/*
 * A016343  168th cyclotomic polynomial.
 * https://oeis.org/A016343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 48, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A016343 – coefficients of the 168th cyclotomic polynomial Φ_168(x)
 *
 *  a(n) = coefficient of xⁿ in Φ_168(x), for n = 0 … φ(168)=48.
 *  The 168th cyclotomic polynomial is given by the Möbius formula
 *
 *         Φ_168(x) = ∏_{d|168} (x^d – 1)^{μ(168/d)}
 *
 *  The non‑zero exponents μ(168/d) are
 *
 *          d =  8 , 12 , 28 , 168   (μ = +1)
 *          d =  4 , 24 , 56 , 84    (μ = -1)
 *
 *  The polynomial is computed once by multiplying the four numerator
 *  factors, dividing exactly by the four denominator factors, and
 *  storing the 49 coefficients.  Subsequent calls simply index the
 *  static table.  All operations use only the allowed arithmetic
 *  operators and are safe for any seq_t (fixed‑width or arbitrary‑precision).
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DEG   216          /* 8+12+28+168 = 216   */
#define ARRAY_SZ  (MAX_DEG+1)  /* maximum coefficient array size */

/* tables of factor degrees */
static const idx_t pos_factors[] = {8,12,28,168};
static const idx_t neg_factors[] = {4,24,56,84};

/* multiply poly by (x^d – 1)  */
static void mul_by_factor(seq_t *poly, size_t *deg, size_t d)
{
    seq_t tmp[ARRAY_SZ] = {0};
    for (size_t i = 0; i <= *deg; ++i) {
        tmp[i]     -= poly[i];     /* coefficient for x^i   */
        tmp[i + d] += poly[i];     /* coefficient for x^(i+d) */
    }
    for (size_t i = 0; i <= *deg + d; ++i) {
        poly[i] = tmp[i];
    }
    *deg += d;
}

/* exact integer polynomial division:  num / den  (den leading coeff = 1) */
static void divide_exact(const seq_t *num, size_t degN,
                         const seq_t *den, size_t degD,
                         seq_t *quot, size_t *degQ)
{
    seq_t rem[ARRAY_SZ];
    for (size_t i = 0; i <= degN; ++i) rem[i] = num[i];

    size_t qdeg = degN - degD;
    *degQ = qdeg;

    for (int i = (int)qdeg; i >= 0; --i) {
        seq_t c = rem[i + degD];
        quot[i] = c;
        for (size_t j = 0; j <= degD; ++j) {
            rem[i + j] -= c * den[j];
        }
    }
}

/* storage for the computed coefficients */
static seq_t coeff[49];
static bool initialized = false;

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* 1. build numerator product (μ = +1) */
        seq_t Pnum[ARRAY_SZ] = {0};
        size_t degPnum = 0;
        Pnum[0] = 1;
        for (size_t i = 0; i < 4; ++i) {
            mul_by_factor(Pnum, &degPnum, pos_factors[i]);
        }

        /* 2. build denominator product (μ = -1) */
        seq_t Pden[ARRAY_SZ] = {0};
        size_t degPden = 0;
        Pden[0] = 1;
        for (size_t i = 0; i < 4; ++i) {
            mul_by_factor(Pden, &degPden, neg_factors[i]);
        }

        /* 3. exact division Pnum / Pden  */
        seq_t tmp[ARRAY_SZ] = {0};
        size_t degRes;
        divide_exact(Pnum, degPnum, Pden, degPden, tmp, &degRes);

        /* 4. copy result into static table */
        for (size_t i = 0; i <= degRes; ++i) coeff[i] = tmp[i];
        initialized = true;
    }

    if ((size_t)n <= 48) return coeff[n];
    return (seq_t)0;
}
