/*
 * A014359  Inverse of 350th cyclotomic polynomial.
 * https://oeis.org/A014359
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

/* OEIS A014359 – inverse of the 350th cyclotomic polynomial.
   Periodic with period 350.
   a(n) = coefficient of x^n in 1/Φ_350(x).  The coefficients of Φ_350(x)
   are computed from ∏_{d|350} (1 - x^d)^{μ(350/d)}.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Degree of Φ_350(x) is φ(350) = 120. */
#define PHI_DEGREE 120

static seq_t phi[PHI_DEGREE + 1];
static seq_t period[350];
static bool initialized = false;

/* multiply A by (1 - x^k) or 1/(1-x^k) according to sign.
   The polynomial A is stored in phi, and the result is written back
   into phi after each factor.  Degree never exceeds PHI_DEGREE. */
static void build_phi(void)
{
    /* start with the polynomial 1 */
    for (int i = 0; i <= PHI_DEGREE; ++i)
        phi[i] = 0;
    phi[0] = 1;

    /* Each entry: divisor k of 350, exponent μ(350/k) (+1 or -1). */
    const struct { unsigned k; int sign; } factors[] = {
        {5,   -1},
        {10,   1},
        {25,   1},
        {35,   1},
        {50,  -1},
        {70,  -1},
        {175, -1},
        {350,  1}
    };
    const size_t nf = sizeof(factors) / sizeof(factors[0]);

    for (size_t idx = 0; idx < nf; ++idx) {
        unsigned k = factors[idx].k;
        int sign = factors[idx].sign;

        /* Temporary array for the new coefficients. */
        seq_t tmp[PHI_DEGREE + 1];
        for (int i = 0; i <= PHI_DEGREE; ++i)
            tmp[i] = 0;

        if (sign == +1) {
            /* Multiply by (1 - x^k). */
            for (int i = 0; i <= PHI_DEGREE; ++i) {
                if (phi[i] == 0) continue;
                tmp[i] += phi[i];               /* +phi[i] */
                if (i + (int)k <= PHI_DEGREE)   /* -phi[i] at i+k */
                    tmp[i + k] -= phi[i];
            }
        } else {
            /* Multiply by 1/(1 - x^k) = 1 + x^k + x^{2k} + ...  (truncated). */
            for (int i = 0; i <= PHI_DEGREE; ++i) {
                if (phi[i] == 0) continue;
                for (int m = 0; i + m * (int)k <= PHI_DEGREE; ++m)
                    tmp[i + m * (int)k] += phi[i];
            }
        }

        /* copy back to phi */
        for (int i = 0; i <= PHI_DEGREE; ++i)
            phi[i] = tmp[i];
    }
}

/* compute one period of the inverse series coefficients: a[n] for 0 ≤ n < 350.
   Use the recurrence a(0)=1,   a(n) = -∑_{k=1}^{min(n,PHI_DEGREE)} phi[k]*a(n-k). */
static void build_period(void)
{
    period[0] = 1;
    for (int n = 1; n < 350; ++n) {
        seq_t s = 0;
        int limit = (n < PHI_DEGREE) ? n : PHI_DEGREE;
        for (int k = 1; k <= limit; ++k)
            s += phi[k] * period[n - k];
        period[n] = -s;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_phi();
        build_period();
        initialized = true;
    }
    return period[n % 350];
}
