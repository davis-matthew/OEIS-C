/*
 * A091209  Primes whose binary representation encodes a polynomial reducible
 *          over GF(2).
 * https://oeis.org/A091209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p whose binary representation encodes a
 *            polynomial reducible over GF(2). The implementation finds primes
 *            one by one and tests reducibility using standard irreducibility
 *            criteria for GF(2)
 * Range:     u8 n <= 27, u16 n <= 5386, u32 n <= 71800, u64 n <= 71800,
 *            u128 n <= 71800, big any n
 * Validated: standard, pass, 79 terms through n = 9512 (2026-11-03)
 * Helpers:   bit_length, bit_length_u128, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include <stddef.h>
#include "../utils/bit_length.h"
#include "../utils/bit_length_u128.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Multiply two polynomials over GF(2) modulo a modulus */
static seq_t poly_mul_mod(seq_t a, seq_t b, seq_t mod) {
    unsigned __int128 prod = 0;
    unsigned __int128 aa = (unsigned __int128)a;
    unsigned __int128 bb = (unsigned __int128)b;
    while (bb) {
        if (bb & 1) prod ^= aa;
        aa <<= 1;
        bb >>= 1;
    }
    /* reduction */
    unsigned int degM = bit_length(mod);
    while (bit_length_u128(prod) >= degM) {
        unsigned int shift = bit_length_u128(prod) - degM;
        prod ^= (unsigned __int128)mod << shift;
    }
    return (seq_t)prod;
}

/* Polynomial remainder div(a,b) over GF(2) */
static seq_t poly_mod(seq_t a, seq_t b) {
    if (!b) return a;
    unsigned int degB = bit_length(b);
    while (bit_length(a) >= degB) {
        unsigned int shift = bit_length(a) - degB;
        a ^= (seq_t)((unsigned __int128)b << shift);
    }
    return a;
}

/* GCD of two polynomials over GF(2) */
static seq_t poly_gcd(seq_t a, seq_t b) {
    while (b) {
        seq_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Compute X^(2^k) mod modPoly; base is X represented as 2 (binary 10) */
static seq_t poly_pow_2k(seq_t base, unsigned int k, seq_t modPoly) {
    seq_t res = base;
    for (unsigned int i = 0; i < k; ++i)
        res = poly_mul_mod(res, res, modPoly);
    return res;
}

/* Test if polynomial (given as binary integer) is reducible over GF(2) */
static bool is_reducible(seq_t poly) {
    if (poly < 4) return false;               /* degree 0 or 1 => irreducible */
    unsigned int d = bit_length(poly) - 1;   /* degree */

    /* factor d into distinct prime divisors */
    unsigned int tmp = d;
    unsigned int primes[16];
    int pcnt = 0;
    for (unsigned int p = 2; p * p <= tmp; ++p)
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    if (tmp > 1) primes[pcnt++] = tmp;

    /* Check condition 1: for every prime divisor q of d */
    for (int i = 0; i < pcnt; ++i) {
        unsigned int q = primes[i];
        unsigned int exp = d / q;
        seq_t y = poly_pow_2k(2, exp, poly);   /* X^(2^exp) mod poly */
        seq_t z = y ^ 2;                        /* y - X over GF(2) */
        if (poly_gcd(z, poly) != 1) return true;/* reducible */
    }

    /* Check condition 2: X^(2^d) ≡ X (mod poly) */
    seq_t y2 = poly_pow_2k(2, d, poly);
    if ((y2 ^ 2) != 0) return true;           /* reducible */

    return false;                              /* irreducible */
}

/* Cache for computed terms */
static seq_t *valid_primes = NULL;
static size_t valid_cap   = 0;
static size_t valid_cnt   = 0;
static uint64_t last_prime_checked = 2; /* Start just below first prime */

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe return */
    /* Generate terms until we have at least n */
    while (valid_cnt < n) {
        last_prime_checked = next_prime(last_prime_checked);
        seq_t p = (seq_t)last_prime_checked;

        if (is_reducible(p)) {
            if (valid_cnt == valid_cap)
                grow_seq((seq_t **)&valid_primes, &valid_cap, valid_cnt + 1);
            valid_primes[valid_cnt++] = p;
        }
    }
    return valid_primes[n - 1];
}
