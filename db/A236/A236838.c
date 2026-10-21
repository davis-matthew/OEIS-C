/*
 * A236838  Numbers whose binary representation encodes a polynomial over
 *          GF(2) with the property that at least one of its irreducible
 *          factors is encoded by a composite number.
 * https://oeis.org/A236838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 880, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------------
  A236838 – Numbers whose binary polynomial has at least one
  irreducible factor whose integer encoding is composite.
  This implementation searches the integers 1,2,3,… in order,
  testing each one until the required nth term is found.
--------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

/* Return the degree of polynomial encoded by x (monic highest bit). */
static inline int poly_deg(seq_t x) {
    unsigned b = bitlen_seq_t(x);
    return b ? (int)b - 1 : -1;
}

/* Polynomial remainder of a divided by b over GF(2). */
static inline seq_t poly_mod(seq_t a, seq_t b) {
    if (b == 0) return a;
    int da = poly_deg(a);
    int db = poly_deg(b);
    while (da >= db && a != 0) {
        unsigned shift = (unsigned)(da - db);
        a ^= (b << shift);
        da = poly_deg(a);
    }
    return a;
}

/* Multiply polynomials a and b modulo mod over GF(2). */
static inline seq_t poly_mul_mod(seq_t a, seq_t b, seq_t mod) {
    seq_t res = 0;
    while (b) {
        if (b & (seq_t)1) res ^= a;
        a <<= 1;
        b >>= 1;
    }
    return poly_mod(res, mod);
}

/* Greatest common divisor over GF(2). */
static inline seq_t poly_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* x^(2^e) mod mod over GF(2). */
static inline seq_t poly_pow2_x_mod(seq_t mod, unsigned e) {
    seq_t r = (seq_t)2;           /* polynomial 'x' = 10b */
    while (e--) r = poly_mul_mod(r, r, mod);
    return r;
}

/* List of primes <= 31 (used in irreducibility test). */
static const unsigned small_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31
};
static const size_t small_primes_cnt = sizeof(small_primes) / sizeof(small_primes[0]);

/* Test whether f is irreducible over GF(2). */
static bool is_irreducible(seq_t f) {
    int deg = poly_deg(f);
    if (deg <= 0) return false;
    for (size_t i = 0; i < small_primes_cnt; ++i) {
        unsigned p = small_primes[i];
        if (p > (unsigned)deg || (deg % p) != 0) continue;
        seq_t g = poly_pow2_x_mod(f, deg / p); /* x^(2^{deg/p}) mod f */
        g ^= (seq_t)2;                        /* minus x */
        if (poly_gcd(g, f) != 1) return false;
    }
    /* Final check: f divides x^{2^deg} - x. */
    seq_t g = poly_pow2_x_mod(f, deg);
    g ^= (seq_t)2;
    return g == 0;
}

/* Polynomial division over GF(2); assumes b divides a. */
static seq_t poly_div(seq_t a, seq_t b) {
    seq_t q = 0;
    int da = poly_deg(a);
    int db = poly_deg(b);
    while (da >= db && a != 0) {
        unsigned shift = (unsigned)(da - db);
        q |= ((seq_t)1 << shift);
        a ^= (b << shift);
        da = poly_deg(a);
    }
    return q;
}

/* Find a non‑trivial monic factor of f (returns 0 if none found). */
static seq_t find_factor(seq_t f) {
    int deg = poly_deg(f);
    if (deg <= 1) return 0;
    for (int d = 1; d <= deg - 1; ++d) {
        seq_t start = ((seq_t)1 << d);          /* leading 1 */
        seq_t end   = ((seq_t)1 << (d + 1));
        for (seq_t g = start; g < end; ++g) {
            if (poly_mod(f, g) == 0) return g;  /* g | f */
        }
    }
    return 0;
}

/* Recursive test: does f have an irreducible factor whose integer
   representation is composite? */
static bool hasCompositeFactor(seq_t f) {
    if (f <= (seq_t)1) return false;
    if (is_irreducible(f)) {
        return !is_prime_big(f);
    }
    seq_t t = find_factor(f);
    if (t == 0) return false;                       /* safety */
    seq_t q = poly_div(f, t);
    return hasCompositeFactor(t) || hasCompositeFactor(q);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t last_candidate = 24;   /* first known term is 25 */

    while (seq_len < n) {
        ++last_candidate;
        if (hasCompositeFactor(last_candidate)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq = tmp;
            }
            seq[seq_len++] = last_candidate;
        }
    }
    return seq[n - 1];
}
