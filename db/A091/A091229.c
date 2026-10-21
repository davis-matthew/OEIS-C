/*
 * A091229  Smallest k such that n+k is irreducible when interpreted as
 *          GF(2)[X]-polynomial.
 * https://oeis.org/A091229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 0 such that n + k is an irreducible
 *            polynomial over GF(2)[X]
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   deg64, poly_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/deg64.h"
#include "../utils/poly_gcd.h"

/* given polynomials a,b and modulus f, compute a * b mod f */
static uint64_t poly_mul_mod(uint64_t a, uint64_t b, uint64_t f, int deg_f) {
    uint64_t prod = 0;
    while (b) {
        if (b & 1ULL) prod ^= a;
        b >>= 1;
        a <<= 1;
        /* reduce a if its degree exceeds deg_f */
        while (deg64(a) > deg_f) {
            a ^= f << (deg64(a) - deg_f);
        }
    }
    /* final reduction of product */
    while (deg64(prod) > deg_f) {
        prod ^= f << (deg64(prod) - deg_f);
    }
    return prod;
}

/* test whether polynomial represented by f (bits 0..deg_f) is irreducible over GF(2) */
static int is_irreducible_uint64(uint64_t f) {
    if (f == 0 || f == 1) return 0;      /* constants are not irreducible */
    int deg_f = deg64(f);
    if (deg_f == 0) return 0;           /* polynomial 1 */

    uint64_t x = 2ULL;                  /* polynomial X */
    for (int i = 1; i <= deg_f; ++i) {
        /* square: x = x^2 mod f */
        x = poly_mul_mod(x, x, f, deg_f);
        if (i <= deg_f / 2) {
            /* check gcd(x - X, f) == 1 */
            uint64_t g = poly_gcd(x ^ 2ULL, f);   /* x - X = x ^ 2 */
            if (g != 1ULL) return 0;
        }
    }
    /* if we reach here, f is irreducible */
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* convert to 64‑bit for internal work (should be safe for n ≤ 10000) */
    uint64_t start = (uint64_t)n;
    for (uint64_t m = start;; ++m) {
        if (is_irreducible_uint64(m)) {
            return (seq_t)(m - start);
        }
    }
}
