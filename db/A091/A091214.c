/*
 * A091214  Composite numbers whose binary representation encodes a polynomial
 *          irreducible over GF(2).
 * https://oeis.org/A091214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite integer whose binary representation,
 *            interpreted as a monic polynomial over GF(2), is irreducible.
 *            Uses Rabin's test for polynomial irreducibility (over GF(2)):
 *            For a polynomial f(x) of degree d, let g = x. For i = 0 ..
 *            ⌊d/2⌋−1 : g ← g² mod f if gcd(f, g − x) ≠ 1 then f r...
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 6995, u64 n <= 9542,
 *            u128 n <= 9542, big any n
 * Helpers:   bitlen_seq_t, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/bitlen_seq_t.h"

/* Polynomial arithmetic over GF(2) – integers represent monic polynomials. */

/* Compute a mod b, treating the arguments as polynomials over GF(2). */
static inline seq_t poly_mod(seq_t a, seq_t b)
{
    if (b == 0) return a;
    unsigned deg_b = bitlen_seq_t(b) - 1;
    seq_t r = a;
    while (r && (bitlen_seq_t(r) - 1) >= deg_b) {
        unsigned shift = (bitlen_seq_t(r) - 1) - deg_b;
        r ^= (b << shift);
    }
    return r;
}

/* GCD of two polynomials over GF(2). */
static inline seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Multiply two monic polynomials, reduced modulo the monic modulus m. */
static inline seq_t poly_mul_mod(seq_t a, seq_t b, seq_t mod)
{
    seq_t prod = 0, aa = a, bb = b;
    while (bb) {
        if (bb & 1) prod ^= aa;
        aa <<= 1;
        bb >>= 1;
    }
    while (prod) {
        unsigned deg_prod = bitlen_seq_t(prod) - 1;
        unsigned deg_mod  = bitlen_seq_t(mod) - 1;
        if (deg_prod < deg_mod) break;
        unsigned shift = deg_prod - deg_mod;
        prod ^= (mod << shift);
    }
    return prod;
}

/* Irreducibility test for a monic polynomial (binary representation). */
static inline bool is_irreducible(seq_t m)
{
    unsigned deg = bitlen_seq_t(m) - 1;    /* degree of m */
    if (deg <= 1) return false;            /* degree 0 or 1 cannot be composite */

    seq_t g = (seq_t)1 << 1;               /* polynomial x */
    unsigned i, limit = deg / 2;
    for (i = 0; i < limit; ++i) {
        g = poly_mul_mod(g, g, m);          /* g = g² mod m */
        seq_t h = g ^ ((seq_t)1 << 1);      /* h = g − x (XOR over GF(2)) */
        seq_t d = poly_gcd(m, h);
        if (d != (seq_t)1) return false;    /* found a non‑trivial factor */
    }
    return true;
}

/* Memoised sieve to find terms of the sequence. */
static seq_t *cache       = NULL;   /* stored terms */
static size_t cache_len   = 0;      /* number of stored terms */
static size_t cache_cap   = 0;      /* allocated capacity */
static idx_t  next_candidate = 4;   /* start at the first composite number */

/* Return the n-th term (1‑based) of the sequence.  Supports any call order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for indices < 1 */

    /* Compute terms up to the requested index. */
    while (cache_len < (size_t)n) {
        /* Search for the next qualifying number. */
        for (;;) {
            /* Skip primes (including 2). */
            if (is_prime(next_candidate)) {
                ++next_candidate;
                continue;
            }

            /* Test irreducibility of the binary polynomial. */
            if (is_irreducible((seq_t)next_candidate)) {
                /* Grow the cache if necessary. */
                if (cache_len >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate;   /* advance search for future calls */
                break;
            }
            ++next_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
