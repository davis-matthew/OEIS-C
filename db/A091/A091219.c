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
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   ilog2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ilog2_u64.h"

/* Remainder of a divided by b over GF(2), b != 0. */
static inline uint64_t poly_mod(uint64_t a, uint64_t b) {
    int db = ilog2_u64(b);
    while (1) {
        int da = ilog2_u64(a);
        if (da < db) break;
        a ^= b << (da - db);
    }
    return a;
}

/* Find a non‑trivial divisor of poly over GF(2).     */
static uint64_t find_factor(uint64_t poly) {
    if (poly <= 1) return poly;
    /* For the sizes that occur in the OEIS tests (n up to ~10⁴),
       a simple linear search is fast. */
    for (uint64_t d = 2; d < poly; ++d) {
        if (poly_mod(poly, d) == 0)
            return d;
    }
    return poly;   /* irreducible */
}

/* Quotient of a divided by b over GF(2), assumes b ≠ 0 and a divisible by b. */
static inline uint64_t poly_div(uint64_t a, uint64_t b) {
    uint64_t q = 0;
    int da = ilog2_u64(a);
    int db = ilog2_u64(b);
    while (da >= db) {
        int shift = da - db;
        q |= (uint64_t)1 << shift;
        a ^= b << shift;
        da = ilog2_u64(a);
    }
    return q;   /* a should be 0 now */
}

/* Recursively factor a polynomial into monic irreducible factors.
   The list of factors (with multiplicity) is stored in 'factors'.
   'cnt' receives the number of factors found. */
static void factor_rec(uint64_t poly, uint64_t *factors, int *cnt) {
    if (poly <= 1) return;
    uint64_t f = find_factor(poly);
    if (f == poly) {
        factors[(*cnt)++] = poly;
    } else {
        factor_rec(f, factors, cnt);
        factor_rec(poly_div(poly, f), factors, cnt);
    }
}

/* Derivative of a polynomial over GF(2). */
static inline uint64_t poly_derivative(uint64_t p) {
    uint64_t d = 0;
    uint64_t rest = p >> 1;          /* skip constant term */
    for (uint64_t i = 0; rest; ++i, rest >>= 1) {
        if (rest & 1) {
            /* original exponent = i+1; if odd → i even → derivative term */
            if ((i & 1) == 0) {
                d |= (uint64_t)1 << i;
            }
        }
    }
    return d;
}

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
