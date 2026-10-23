/*
 * A014924  Inverse of 915th cyclotomic polynomial.
 * https://oeis.org/A014924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series expansion
 *            of 1/Φ_915(x); the sequence is periodic with period 915, and
 *            a(n) = a(n mod 915)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the cyclotomic polynomial factorisation
   Φ_915(x) = ∏_{d∣915} (x^d−1)^{µ(915/d)} and builds the reciprocal series
   term by term up to one full period, storing the result in a static array. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* one‑period coefficients of the sequence (length 915) */
static seq_t period_a[915];
static seq_t period_tmp[915];
static bool period_inited = false;

/* initialise period_a only once */
static void init_period_a(void)
{
    if (period_inited)
        return;

    /* start with the series 1 */
    for (size_t i = 0; i < 915; ++i)
        period_a[i] = (seq_t)0;
    period_a[0] = (seq_t)1;

    /* divisors of 915 in ascending order and corresponding μ(915/d) */
    static const int divisors[8] = { 1, 3, 5, 15, 61, 183, 305, 915 };
    static const int mus[8]    = { -1,  1,  1, -1,  1, -1, -1,   1 };
    const int ND = 8;

    for (int idx = 0; idx < ND; ++idx)
    {
        const int d  = divisors[idx];
        const int mu = mus[idx];

        /* copy current series to a temporary buffer */
        for (size_t i = 0; i < 915; ++i)
            period_tmp[i] = period_a[i];

        if (mu == -1)
        {
            /* multiply by the finite polynomial (1 - x^d) */
            for (size_t i = d; i < 915; ++i)
                period_tmp[i] -= period_a[i - d];
        }
        else /* mu == +1, multiply by the infinite series (1 - x^d)^(-1) */
        {
            /* accumulate along residue classes modulo d */
            for (size_t i = d; i < 915; ++i)
                period_tmp[i] += period_tmp[i - d];
        }

        /* copy the updated series back */
        for (size_t i = 0; i < 915; ++i)
            period_a[i] = period_tmp[i];
    }

    period_inited = true;
}

seq_t a(idx_t n)
{
    init_period_a();
    /* period is 915, n is unsigned so the modulo is well defined */
    return period_a[(size_t)(n % (idx_t)915)];
}
