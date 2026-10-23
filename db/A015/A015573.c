/*
 * A015573  Inverse of 1564th cyclotomic polynomial.
 * https://oeis.org/A015573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic inverse‐series of the 1564th cyclotomic polynomial.
   a(n) is the coefficient of xⁿ in 1 / Φ₁₅₆₄(x).  The sequence
   is periodic with period 1564.  We compute it by multiplying
   (1 – x^d)^{e(d)} for each divisor d of 1564, where
   e(d) = –μ(1564 / d).  The factor 1/(1–x^d) is handled by a
   recurrence new[i] = old[i] + new[i–d] (mod period). */

#include <stdbool.h>
#include <stddef.h>

#define PERIOD 1564U

/* Exponents for the divisors of 1564:
   e(d) = –μ(1564 / d).  */
static const struct { idx_t d; int exp; } facs[] = {
    {  2U,  +1},
    {  4U,  -1},
    { 34U,  -1},
    { 46U,  -1},
    { 68U,  +1},
    { 92U,  +1},
    { 782U, +1},
    {1564U, -1}
};
static const int nf = (int)(sizeof(facs) / sizeof(facs[0]));

static seq_t coeff[PERIOD];
static bool initialized = false;

static void init_periodic(void)
{
    /* start with the constant 1 */
    for (idx_t i = 0; i < PERIOD; ++i) coeff[i] = 0;
    coeff[0] = 1;

    seq_t tmp[PERIOD];
    for (int f = 0; f < nf; ++f) {
        idx_t d = facs[f].d;
        int   e = facs[f].exp;

        if (e == +1) {
            /* multiply by (1 – x^d) */
            for (idx_t i = 0; i < PERIOD; ++i) {
                tmp[i] = coeff[i];
                if (i >= d) tmp[i] -= coeff[i - d];
            }
        } else { /* e == -1 : multiply by 1/(1 – x^d) */
            for (idx_t i = 0; i < PERIOD; ++i) {
                tmp[i] = coeff[i];
                if (i >= d) tmp[i] += tmp[i - d]; /* recurrence */
            }
        }
        /* prepare for next factor */
        for (idx_t i = 0; i < PERIOD; ++i) coeff[i] = tmp[i];
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_periodic();
        initialized = true;
    }
    return coeff[n % PERIOD];
}
