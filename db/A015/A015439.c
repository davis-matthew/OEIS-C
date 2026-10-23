/*
 * A015439  Inverse of 1430th cyclotomic polynomial.
 * https://oeis.org/A015439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   _moi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n>0,
   a[n] = -∑_{k=1}^{480} Φ[k] * a[n-k]   (Φ is the 1430th cyclotomic polynomial).
   Period: 1430. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_moi.h"

#define PERIOD (1430u)
#define DEG    (480u)
#define N_VAL   (1430u)

/* compute coefficients of Φ_N(x) = ∏_{d|N} (x^d - 1)^{μ(N/d)}   (finite polynomial of degree DEG) */
static void _compute_phi_coeff(seq_t coeff[DEG + 1])
{
    seq_t poly[DEG + 1] = {0};
    seq_t newpoly[DEG + 1] = {0};
    poly[0] = 1;                      /* start with 1 */

    /* enumerate divisors of N_VAL */
    for (idx_t d = 1; d <= N_VAL; ++d) {
        if (N_VAL % d != 0)
            continue;
        int mu = _moi(N_VAL / d);
        if (mu == 1) {                /* multiply by (x^d - 1) */
            for (idx_t i = 0; i <= DEG; ++i) {
                newpoly[i] = poly[i];
                if (i >= d)
                    newpoly[i] -= poly[i - d];
            }
        } else if (mu == -1) {        /* multiply by -(1 + x^d + x^{2d} + …) */
            for (idx_t i = 0; i <= DEG; ++i)
                newpoly[i] = 0;
            for (idx_t k = 0; k * d <= DEG; ++k) {
                idx_t shift = k * d;
                for (idx_t i = 0; i + shift <= DEG; ++i)
                    newpoly[i + shift] -= poly[i];
            }
        } else {                      /* mu == 0, factor is 1, skip */
            continue;
        }
        /* copy back */
        for (idx_t i = 0; i <= DEG; ++i)
            poly[i] = newpoly[i];
    }
    for (idx_t i = 0; i <= DEG; ++i)
        coeff[i] = poly[i];
}

/* initialize period array once */
static void _init_period(seq_t period[PERIOD])
{
    static bool done = false;
    if (done)
        return;
    done = true;

    /* compute Φ coefficients */
    seq_t phi[DEG + 1];
    _compute_phi_coeff(phi);

    /* build the periodic sequence */
    period[0] = 1;
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        idx_t lim = (n < DEG) ? n : DEG;
        for (idx_t i = 1; i <= lim; ++i)
            sum += phi[i] * period[n - i];
        period[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    static seq_t period[PERIOD];
    _init_period(period);
    return period[n % PERIOD];
}
