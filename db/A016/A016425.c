/*
 * A016425  497th cyclotomic polynomial.
 * https://oeis.org/A016425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the cyclotomic polynomial Φ_497(x)
 * Range:     i8 n <= 420, i16 n <= 420, i32 n <= 420, i64 n <= 420,
 *            i128 n <= 420, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Φ_497(x) = (x^497‑1)(x‑1) / ((x^71‑1)(x^7‑1))                            */
/* The result is a symmetric integer polynomial of degree φ(497)=420.        */

#include <stdbool.h>
#include <stddef.h>

static seq_t coeff[421];      /* coefficients a(0)…a(420) */
static bool initialized = false;

/* Compute the coefficients of Φ_497(x) once. */
static void compute_coeffs(void)
{
    /* Remainder polynomial N(x) = (x^497‑1)(x‑1) = x^498 - x^497 - x + 1 */
    seq_t rem[499];
    for (int i = 0; i < 499; ++i) rem[i] = 0;
    rem[0] = 1;     /* constant term */
    rem[1] = -1;    /* -x */
    rem[497] = -1;  /* -x^497 */
    rem[498] = 1;   /* +x^498 */

    seq_t Q[421];
    for (int i = 0; i < 421; ++i) Q[i] = 0;

    /* Denominator D(x) = (x^71‑1)(x^7‑1) = x^78 - x^71 - x^7 + 1   */
    /* D[0]  =  1,  D[7]  = -1,  D[71] = -1,  D[78] =  1           */

    for (int i = 498; i >= 78; --i) {
        seq_t coeff_i = rem[i];
        if (coeff_i != 0) {
            Q[i - 78] = coeff_i;   /* coefficient of Q at degree i‑78 */

            /* subtract coeff_i * D(x) shifted by (i‑78) */
            rem[i]     -= coeff_i;                /* D[0] =  1  */
            rem[i - 7] += coeff_i;                /* D[7] = -1  */
            rem[i - 71] += coeff_i;               /* D[71] = -1 */
            rem[i - 78] -= coeff_i;               /* D[78] =  1  */
        }
    }

    /* Copy result into global coefficient array */
    for (int i = 0; i < 421; ++i) coeff[i] = Q[i];
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) compute_coeffs();
    if (n > 420) return 0;
    return coeff[n];
}
