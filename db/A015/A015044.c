/*
 * A015044  Inverse of 1035th cyclotomic polynomial.
 * https://oeis.org/A015044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of xⁿ in the power‑series expansion of *
 *            the reciprocal of the 1035‑th cyclotomic polynomial * Φ₁₀₃₅⁻¹(x)
 *            . * The sequence is periodic with period 1035
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   compute_phi_rec, divide_poly, init_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_phi_rec.h"
#include "../utils/divide_poly.h"
#include "../utils/init_cache.h"

/* Forward declarations of helper functions */
static int  compute_phi_rec(uint64_t n, long long **out, int *deg);
static void divide_poly(const long long *num, int deg_num,
                        const long long *div, int deg_div,
                        long long *quo, int *deg_quo);

/* -------------------------------------------------------------------------- */
/*  Recursive computation of a single cyclotomic polynomial Φ_n   */

/* -------------------------------------------------------------------------- */
/*  Generic polynomial division (num / div)  */
/*  Both polynomials are monic; division is exact.         */

/* -------------------------------------------------------------------------- */
/*  One‑time initialization of the period and cache  */

/* -------------------------------------------------------------------------- */
/*  Required public function  */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t cache[1035];
    static int  initialized = 0;

    if (!initialized) {
        init_cache(cache);
        initialized = 1;
    }
    const uint64_t PERIOD = 1035;
    return cache[(unsigned)(n % PERIOD)];
}
