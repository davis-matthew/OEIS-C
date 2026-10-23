/*
 * A015389  Inverse of 1380th cyclotomic polynomial.
 * https://oeis.org/A015389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n-th coefficient of the power series expansion of
 *            1/Φ1380(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   _build_inverse_series, _divisors_uint64, _fetch_phi_1380,
 *            _poly_divide_monic_in_place
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the formal inverse of the 1380‑th cyclotomic polynomial.      */
/* The sequence is periodic with period 1380; this implementation precomputes
   the first 1380 coefficients once and then returns the requested term. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_build_inverse_series.h"
#include "../utils/_divisors_uint64.h"
#include "../utils/_fetch_phi_1380.h"
#include "../utils/_poly_divide_monic_in_place.h"

/* ------------------------------------------------------------------------- */
/* Helper: compute all divisors of N (here N = 1380) and sort them.          */

/* ------------------------------------------------------------------------- */
/* Divide monic polynomial 'num' by monic polynomial 'den', modifying 'num'
   to contain the quotient.  After division, *num_deg is updated.             */

/* ------------------------------------------------------------------------- */
/* Compute Φ1380(x) by the recursive definition:                           */
/* Φ_n(x) = (x^n – 1) / ∏_{d|n, d < n} Φ_d(x).  All polynomials are stored
   with degree increasing: coefficient[0] = constant term.                  */

/* ------------------------------------------------------------------------- */
/* Compute the first 1380 coefficients of the series 1/Φ1380(x).              */
/* Uses Newton--Raphson iteration for power‑series inversion.                 */

/* ------------------------------------------------------------------------- */
/* Public function required by the build system.                               */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t cache[1380];
    static bool computed = false;
    if (!computed) {
        _build_inverse_series(cache);
        computed = true;
    }
    size_t idx = (size_t)(n % 1380);
    return cache[idx];
}
