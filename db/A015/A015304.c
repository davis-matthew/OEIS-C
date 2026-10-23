/*
 * A015304  Inverse of 1295th cyclotomic polynomial.
 * https://oeis.org/A015304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   build_phi_1295, poly_div_by_xk_minus_1, poly_mul_by_xk_minus_1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A015304 – Inverse of the 1295th cyclotomic polynomial.
 *
 *  The sequence is the coefficient of x^n in the formal power series
 *  1/Φ_1295(x).  The coefficients are periodic with period 1295.
 *
 *  The implementation computes Φ_1295(x) as a product of
 *  (x^m-1) raised to ±1, using the divisor/möbius representation.
 *  It then uses the recurrence
 *
 *      a(0) = 1,
 *      a(k) = -∑_{i=1}^{min(k,deg)} Φ_1295[i] * a(k-i)    for k≥1,
 *
 *  where Φ_1295[i] are the coefficients of Φ_1295(x) and
 *  deg = Φ_1295 degree = 864.
 *
 *  The sequence values are cached in a static array of length 1295
 *  (the period), so that each call runs in O(1) time after the
 *  one‑time initialization.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_phi_1295.h"
#include "../utils/poly_div_by_xk_minus_1.h"
#include "../utils/poly_mul_by_xk_minus_1.h"

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Main entry function: sequence value a(n). */
seq_t a(idx_t n)
{
    /* Period length declared constant in OEIS */
    const idx_t PERIOD = 1295;

    /* Static cache for first PERIOD terms */
    static seq_t *period_cache = NULL;
    static bool initialized   = false;

    if (!initialized) {
        /* 1. Compute Φ_1295(x) */
        seq_t *phi = NULL;
        size_t phi_len = 0;
        build_phi_1295(&phi, &phi_len);
        /* phi[0] should be 1 and phi_len-1 = 864 */

        /* 2. Compute inverse series coefficients a[0..PERIOD-1] */
        period_cache = (seq_t *)calloc(PERIOD, sizeof(seq_t));

        period_cache[0] = 1;                  /* a(0) = 1 */
        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t sum = 0;
            size_t limit = (phi_len - 1 < (size_t)k) ? (phi_len - 1) : (size_t)k;
            for (size_t i = 1; i <= limit; ++i) {
                sum += phi[i] * period_cache[k - (idx_t)i];
            }
            period_cache[k] = -sum;
        }

        /* 3. Free temporary polynomial */
        free(phi);

        initialized = true;
    }

    idx_t idx = n % PERIOD;
    return period_cache[idx];
}
