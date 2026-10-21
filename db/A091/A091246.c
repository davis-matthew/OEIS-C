/*
 * A091246  Inverse function of A091242: position in A091242 if the n-th
 *          GF(2)[X] polynomial is reducible, 0 otherwise.
 * https://oeis.org/A091246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   degree_uint64, nth_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A091246 - Inverse function of A091242
 *  a(n) = number of reducible monic GF(2)[x] polynomials
 *         with positions ≤ n in the usual enumeration
 *         (degree increasing, then lexicographic).
 *  if the n-th polynomial is irreducible, a(n) = 0
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/degree_uint64.h"
#include "../utils/nth_poly.h"

/* multiply two polynomials over GF(2), results fit in 64 bits
 * (the degrees we need are < 64).  Works for any seq_t,
 * because we always cast to (uint64_t) before using. */
static inline uint64_t poly_mul(uint64_t a, uint64_t b)
{
    uint64_t r = 0;
    while (b) {
        if (b & 1u) r ^= a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}

/* remainder of a divided by mod (both monic). */
static inline uint64_t poly_mod(uint64_t a, uint64_t mod)
{
    unsigned int dmod = degree_uint64(mod);
    while (a) {
        unsigned int da = degree_uint64(a);
        if (da < dmod) break;
        uint64_t shift = da - dmod;
        a ^= (mod << shift);
    }
    return a;
}

/* greatest common divisor over GF(2) */
static inline uint64_t poly_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Test if a monic polynomial f is reducible over GF(2). */
static bool is_reducible(uint64_t f)
{
    /* constant polynomials (1) are considered irreducible */
    if (f <= 1u) return false;
    unsigned int d = degree_uint64(f);
    if (d <= 1) return false;   /* linear polynomials are irreducible */

    uint64_t xpow = 2u; /* x */
    for (unsigned int i = 1; i <= d / 2; ++i) {
        /* compute x^(2^i) mod f */
        uint64_t sq = poly_mul(xpow, xpow);
        xpow = poly_mod(sq, f);
        /* compute gcd(f, x^(2^i) - x) */
        uint64_t t = xpow ^ 2u;    /* subtraction is xor */
        uint64_t g = poly_gcd(f, t);
        if (g != 1u) return true; /* reducible */
    }
    return false;   /* no factor found */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Determine if the n‑th polynomial is reducible. */
    uint64_t f = nth_poly(n);
    if (!is_reducible(f)) return 0;

    /* Count reducible polynomials up to and including n. */
    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        if (is_reducible(nth_poly(m))) ++count;
    }
    return count;
}
