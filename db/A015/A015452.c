/*
 * A015452  Inverse of 1443rd cyclotomic polynomial.
 * https://oeis.org/A015452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   div_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of 1443rd cyclotomic polynomial – coefficients of 1/Φ₁₄₄₃(x).
   The sequence is periodic with period 1443.  The value a(n) is the
   coefficient of xⁿ in the power‐series expansion of 1/Φ₁₄₄₃(x). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/div_poly.h"

/* 1443 = 3·13·37  ->  φ(1443) = 864  */
#define NUF 1496   /* numerator maximum degree (sum of 3,13,37,1443) */
#define DUF 632    /* denominator maximum degree (sum of 39,111,481,1) */
#define DEG_P 864  /* result degree */
#define PERIOD 1443

static bool _initialized = false;
static seq_t _p[DEG_P + 1];      /* Φ₁₄₄₃ coefficients (degree 864) */
static seq_t _q[PERIOD];         /* inverse series coefficients */

/* multiply a polynomial by (x^k - 1) in place.
   poly   : array of size MAX, the polynomial is stored in poly[0..deg]
   back    : degree of the polynomial before the multiplication
   k        : exponent of the factor
   maxdeg   : maximum possible degree after multiplication
   tmp      : temporary array of size maxdeg+1, re‑used for evaluation */
static void mul_by_factor(seq_t *poly, int *deg, int k, int maxdeg, seq_t *tmp)
{
    int d = *deg;
    for (int i = 0; i <= maxdeg; ++i) tmp[i] = 0;
    for (int i = 0; i <= d; ++i) {
        tmp[i] -= poly[i];     /* coefficient * (-1) at same degree */
        tmp[i + k] += poly[i]; /* coefficient * 1 at degree + k */
    }
    for (int i = 0; i <= d + k; ++i) poly[i] = tmp[i];
    *deg = d + k;
}

seq_t a(idx_t n)
{
    if (!_initialized) {
        /* ---------- Build numerator polynomial ----------
           numerator = (x^3 - 1)(x^13 - 1)(x^37 - 1)(x^1443 - 1) */
        seq_t numer[NUF + 1];
        seq_t denom[DUF + 1];
        seq_t tmp[NUF + 1];   /* temporary buffer for multiplication */
        for (int i = 0; i <= NUF; ++i) numer[i] = 0;
        numer[0] = 1;
        int degN = 0;
        mul_by_factor(numer, &degN, 3, NUF, tmp);
        mul_by_factor(numer, &degN, 13, NUF, tmp);
        mul_by_factor(numer, &degN, 37, NUF, tmp);
        mul_by_factor(numer, &degN, 1443, NUF, tmp);

        /* ---------- Build denominator polynomial ----------
           denominator = (x^39 - 1)(x^111 - 1)(x^481 - 1)(x - 1) */
        for (int i = 0; i <= DUF; ++i) denom[i] = 0;
        denom[0] = 1;
        int degD = 0;
        mul_by_factor(denom, &degD, 39, DUF, tmp);
        mul_by_factor(denom, &degD, 111, DUF, tmp);
        mul_by_factor(denom, &degD, 481, DUF, tmp);
        mul_by_factor(denom, &degD, 1, DUF, tmp);

        /* ---------- Polynomial division ----------
           result polynomial P(x) = numer / denom  (deg 864) */
        seq_t res[DEG_P + 1];
        div_poly(numer, degN, denom, degD, res);
        for (int i = 0; i <= DEG_P; ++i) _p[i] = res[i];

        /* ---------- Compute inverse series ----------
           Q(x) = 1 / Φ₁₄₄₃(x) mod x^PERIOD */
        _q[0] = 1;
        for (int m = 1; m < PERIOD; ++m) {
            seq_t sum = 0;
            int up = (m < DEG_P) ? m : DEG_P;
            for (int i = 1; i <= up; ++i) {
                sum += _p[i] * _q[m - i];
            }
            _q[m] = -sum;
        }
        _initialized = true;
    }

    idx_t idx = n % PERIOD;
    return _q[idx];
}
