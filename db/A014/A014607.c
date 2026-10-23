/*
 * A014607  Inverse of 598th cyclotomic polynomial.
 * https://oeis.org/A014607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A014607:  inverse of the 598th cyclotomic polynomial
    The terms a(n) are the coefficients of the formal power series
    expansion of 1/Φ₅₉₈(x); they form a periodic sequence of period 598. */

#include <stddef.h>
#include <stdbool.h>

/* Number of terms that repeat. */
#define PERIOD 598

/* Maximum degree we have to handle during the intermediate
   polynomial calculations. 636 (degree of the numerator) and
   372 (degree of the denominator) are the largest degrees.  */
#define MAX_DEG 700
/* The degree of Φ₅₉₈ is 264. */
#define DEG_PHI 264

/* Multiply a polynomial 'poly' by (1 - x^d).
   poly has current degree 'deg'; after the call its degree becomes
   deg + d.  The array big enough to hold up to MAX_DEG coefficients
   is assumed. */
static void mul_xd_minus1(seq_t *poly, size_t *deg, idx_t d)
{
    static seq_t tmp[MAX_DEG];
    size_t new_deg = *deg + (size_t)d;

    /* Clear temporary array. */
    for (size_t i = 0; i <= new_deg; ++i)
        tmp[i] = 0;

    /* (1 - x^d) * oldPoly */
    for (size_t i = 0; i <= *deg; ++i) {
        tmp[i]     += poly[i];
        tmp[i + d] -= poly[i];
    }

    /* Copy back the result. */
    for (size_t i = 0; i <= new_deg; ++i)
        poly[i] = tmp[i];

    *deg = new_deg;
}

seq_t a(idx_t n)
{
    /* Static cache for the periodic values.  */
    static seq_t cache[PERIOD];
    static bool init = false;

    if (!init) {
        /* ---------- 1.  Build Φ₅₉₈(x) ----------
           Φ₅₉₈(x) = Π_{d|598} (x^d - 1)^{μ(598/d)}  */
        static seq_t Q[MAX_DEG];          /* product for μ=+1  */
        static seq_t R[MAX_DEG];          /* product for μ=-1  */
        size_t degQ = 0, degR = 0;
        Q[0] = R[0] = 1;

        /* divisors with μ(598/d)=+1  */
        const idx_t posDiv[4] = {2, 13, 23, 598};
        for (int i = 0; i < 4; ++i)
            mul_xd_minus1(Q, &degQ, posDiv[i]);

        /* divisors with μ(598/d)=-1  */
        const idx_t negDiv[4] = {1, 26, 46, 299};
        for (int i = 0; i < 4; ++i)
            mul_xd_minus1(R, &degR, negDiv[i]);

        /* ---------- 2.  Divide Q by R to get Φ₅₉₈ ----------
           The division is exact because Φ₅₉₈ is a cyclotomic polynomial. */
        static seq_t Phi[DEG_PHI + 1];
        for (size_t i = 0; i <= DEG_PHI; ++i)
            Phi[i] = 0;

        /* Classic polynomial long division (R[0] == 1). */
        for (size_t i = degQ; ; --i) {
            seq_t coef = Q[i];
            size_t idx  = i - degR;
            Phi[idx] = coef;

            for (size_t j = 0; j <= degR; ++j) {
                Q[i - j] -= coef * R[j];
            }

            if (i == degR)
                break;
        }

        /* ---------- 3.  Compute the first PERIOD coefficients of 1/Φ₅₉₈ ----------
           a(0)=1; a(n) = -Σ_{k=1..min(n,DEG_PHI)} Φ[k] * a(n-k)  */
        static seq_t tmpA[PERIOD];
        tmpA[0] = 1;
        for (idx_t m = 1; m < PERIOD; ++m) {
            seq_t sum = 0;
            size_t limit = (m <= DEG_PHI) ? (size_t)m : (size_t)DEG_PHI;

            for (size_t k = 1; k <= limit; ++k) {
                sum += Phi[k] * tmpA[m - k];
            }
            tmpA[m] = -sum;
        }

        /* Store the computed values in the permanent cache. */
        for (idx_t i = 0; i < PERIOD; ++i)
            cache[i] = tmpA[i];

        init = true;
    }

    return cache[n % PERIOD];
}
