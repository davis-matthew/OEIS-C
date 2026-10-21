/*
 * A091219  Moebius-analog for the domain GF(2)[X]: a(n)=0 if
 *          A091221(n)!=A091222(n) (i.e., if the polynomial is not
 *          squarefree), otherwise (-1)^A091222(n).
 * https://oeis.org/A091219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if the GF(2)[X] polynomial represented by n is not
 *            squarefree, * otherwise (-1)^(# of distinct irreducible factors)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   deg_uint64, factor_rec, find_factor, poly_derivative, poly_div,
 *            poly_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/deg_uint64.h"
#include "../utils/factor_rec.h"
#include "../utils/find_factor.h"
#include "../utils/poly_derivative.h"
#include "../utils/poly_div.h"
#include "../utils/poly_mod.h"

/* Greatest common divisor of polynomials a and b over GF(2). */
static inline uint64_t poly_gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    uint64_t poly = n;

    /* Bypass the square‑free test for the unit polynomial. */
    if (poly != 1) {
        uint64_t deriv = poly_derivative(poly);
        if (deriv == 0)               /* derivative zero ⇒ polynomial is a square */
            return (seq_t)0;
        if (poly_gcd(poly, deriv) != 1)  /* common factor with derivative */
            return (seq_t)0;
    }

    /* Squarefree – count distinct irreducible factors. */
    uint64_t factors[64];
    int fac_cnt = 0;
    factor_rec(poly, factors, &fac_cnt);

    /* Remove duplicates (there should be none for a square‐free polynomial). */
    int distinct = 0;
    for (int i = 0; i < fac_cnt; ++i) {
        bool seen = false;
        for (int j = 0; j < i; ++j) {
            if (factors[j] == factors[i]) { seen = true; break; }
        }
        if (!seen) ++distinct;
    }

    return ((distinct & 1) ? (seq_t)-1 : (seq_t)1);
}
