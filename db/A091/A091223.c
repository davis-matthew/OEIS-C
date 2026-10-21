/*
 * A091223  Differences between consecutive irreducible GF(2)[X]-polynomials.
 * https://oeis.org/A091223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between the n‑th and (n-1)-th monic
 *            irreducible polynomial over GF(2) (binary representation)
 * Range:     u8 n <= 7, u16 n <= 70, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   seq_bitlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_bitlen.h"

/* ----- Basic helpers --------------------------------------------------- */

/* polynomial remainder: a mod b  (b ≠ 0) */
static seq_t poly_mod(seq_t a, seq_t b)
{
    if (b == 0) return a;
    idx_t deg_b = seq_bitlen(b);
    while (true) {
        idx_t deg_a = seq_bitlen(a);
        if (deg_a < deg_b || a == 0) break;
        idx_t shift = deg_a - deg_b;
        a ^= b << shift;
    }
    return a;
}

/* polynomial gcd */
static seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* polynomial multiplication followed by reduction modulo mod */
static seq_t poly_mul_mod(seq_t a, seq_t b, seq_t mod)
{
    seq_t prod = 0;
    idx_t j = 0;
    seq_t bb = b;
    while (bb) {
        if (bb & (seq_t)1) {
            prod ^= a << j;
        }
        bb >>= 1;
        j++;
    }
    return poly_mod(prod, mod);
}

/* prime factorisation of a small integer (n ≤ 2^64) */
static void prime_factors(idx_t n, idx_t *fac, size_t *cnt)
{
    *cnt = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            fac[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) fac[(*cnt)++] = n;
}

/* ----- Irreducible polynomial test ----------------------------------- */

static bool is_irreducible(seq_t f)
{
    /* reject constants and zero */
    if (f <= (seq_t)1) return false;

    idx_t deg = seq_bitlen(f) - 1;
    if (deg == 1) return true;          /* x and x+1 are irreducible */

    /* factorise the degree */
    idx_t pf[64];
    size_t pf_cnt;
    prime_factors(deg, pf, &pf_cnt);

    const seq_t x = (seq_t)2;           /* polynomial x */
    const seq_t one = (seq_t)1;

    /* compute x^(2^deg) mod f */
    seq_t t = x;
    for (idx_t i = 0; i < deg; ++i)
        t = poly_mul_mod(t, t, f);
    if (t != x) return false;

    /* Frobenius test for each prime divisor of deg */
    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t p = pf[i];
        idx_t exp = deg / p;           /* number of squarings */
        seq_t g = x;
        for (idx_t j = 0; j < exp; ++j)
            g = poly_mul_mod(g, g, f);

        seq_t h = g ^ x;               /* g - x  (XOR) */
        seq_t d = poly_gcd(h, f);
        if (d != one) return false;
    }
    return true;
}

/* ----- Cache of irreducible polynomials ------------------------------ */

static seq_t *irreducibles = NULL;
static size_t irreducibles_len   = 0;
static size_t irreducibles_cap   = 0;
static idx_t  next_candidate     = 2;

/* ensure that the cache contains at least 'needed' irreducibles (1‑based) */
static void ensure_irreducibles(size_t needed)
{
    if (irreducibles_len >= needed) return;

    /* grow buffer if necessary */
    if (irreducibles_cap < needed) {
        size_t new_cap = irreducibles_cap ? irreducibles_cap << 1 : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(irreducibles, new_cap * sizeof(seq_t));
        irreducibles     = tmp;
        irreducibles_cap = new_cap;
    }

    /* search for further irreducible polynomials */
    while (irreducibles_len < needed) {
        seq_t cand = (seq_t)next_candidate;
        if (is_irreducible(cand))
            irreducibles[irreducibles_len++] = cand;
        ++next_candidate;
    }
}

/* ----- Public entry point -------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for offset 0 */

    ensure_irreducibles((size_t)n + 1);
    return irreducibles[(size_t)n] - irreducibles[(size_t)n - 1];
}
