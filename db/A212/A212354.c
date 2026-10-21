/*
 * A212354  a(n) is the second smallest positive incongruent solutions of the
 *          congruence x^2 + (x+1)^2 == 0 (mod prime), where prime =
 *          A002144(n) (Pythagorean primes).
 * https://oeis.org/A212354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second smallest positive solution of 2*x^2 + 2*x + 1 ≡ 0
 *            (mod p) where p is the nth prime ≡ 1 (mod 4)
 * Range:     u8 n <= 24, u16 n <= 3315, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation finds p, computes a primitive root g, obtains
   r = g^((p-1)/4) ≡ √‑1 (mod p), then solves for the two roots
   x = (‑1 ± r)/2 (mod p) and returns the larger one. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* ------------------------------------------------------------------ */
/* Modular multiplication that works for seq_t up to 64 bits.  If the
   compiler provides __int128, we use it to avoid overflow; otherwise
   a binary multiplication fallback is used. */
static inline seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__)
    return ( (unsigned __int128)a * b ) % mod;
#else
    seq_t result = 0;
    seq_t base   = a % mod;
    seq_t exp    = b;
    while (exp > 0) {
        if (exp & 1) {
            result += base;
            result %= mod;
        }
        exp >>= 1;
        base <<= 1;
        base %= mod;
    }
    return result;
#endif
}

/* ------------------------------------------------------------------ */
/* Dynamic array of primes ≡ 1 mod 4.  Pointers are kept static so
   consecutive calls can reuse the data. */
static seq_t      *primes_1mod4 = NULL;
static size_t     primes_cap   = 0;
static idx_t      primes_cnt   = 0;   /* number stored */
static idx_t      last_checked = 1;   /* last candidate tested   */

static void ensure_primes_capacity(size_t need)
{
    if (primes_cap >= need) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(primes_1mod4, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    primes_1mod4 = tmp;
    primes_cap   = new_cap;
}

/* Return the nth prime ≡ 1 (mod 4)  (n ≥ 1). */
static seq_t prime_1mod4_at(idx_t n)
{
    if (n < 1) return 0;
    while (primes_cnt < n) {
        /* search next odd candidate starting from last_checked+1 */
        idx_t cand = last_checked + 1;
        if (cand <= 2) cand = 3;
        if ((cand & 1) == 0) cand++;             /* make odd */
        /* loop until found a suitable prime */
        while (true) {
            if ((cand & 3ULL) != 1) {            /* not ≡ 1 (mod 4) */
                cand += 2;
                last_checked = cand;
                continue;
            }
            if (is_prime(cand)) {                /* prime found */
                ensure_primes_capacity(primes_cnt + 1);
                primes_1mod4[primes_cnt++] = cand;
                last_checked = cand;
                break;
            }
            cand += 2;
            last_checked = cand;
        }
    }
    return primes_1mod4[n-1];
}

/* ------------------------------------------------------------------ */
/* Factor m (<= 2^64) into its distinct prime factors.  The list
   occupies the array ‘factors’ and the count is written to ‘cnt’.
   m must be > 1. */
static void factor_small(seq_t m, seq_t *factors, size_t *cnt)
{
    seq_t t = m;
    *cnt = 0;
    for (seq_t p = 2; (p * p) <= t; ++p) {
        if (t % p == 0) {
            factors[(*cnt)++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) factors[(*cnt)++] = t;
}

/* ------------------------------------------------------------------ */
/* Find a primitive root modulo prime p.  The algorithm tests small
   integers g and verifies that g^((p-1)/q) ≠ 1 for every prime factor
   q of p‑1.  It is guaranteed to find one because p is prime. */
static seq_t primitive_root(seq_t p)
{
    seq_t factors[64];
    size_t cf = 0;
    factor_small(p - 1, factors, &cf);

    for (seq_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < cf && ok; ++i) {
            seq_t exp = (p - 1) / factors[i];
            seq_t r   = mod_pow(g, (uint64_t)exp, p);
            if (r == 1) ok = false;
        }
        if (ok) return g;
    }
    return 0; /* never reached for a prime */
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined */
    seq_t p = prime_1mod4_at(n);       /* p ≡ 1 (mod 4) */

    /* Inverse of 2 modulo p : (p+1)/2 */
    seq_t inv2 = (p + 1) / 2;

    /* Primitive root g of p */
    seq_t g = primitive_root(p);
    if (g == 0) return 0;              /* safety, should not happen */

    /* r = g^((p-1)/4) ≡ √‑1 (mod p) */
    seq_t exp_r = (p - 1) / 4;
    seq_t r = mod_pow(g, (uint64_t)exp_r, p);

    /* Compute the two roots: x = (-1 ± r)/2 (mod p) */
    seq_t diff1 = r - 1;                 /* r − 1  (0 ≤ diff1 < p) */
    seq_t diff2 = p - r - 1;             /* p − r − 1 (0 ≤ diff2 < p) */

    seq_t x1 = mul_mod_seq(diff1, inv2, p);
    seq_t x2 = mul_mod_seq(diff2, inv2, p);

    seq_t small = (x1 < x2) ? x1 : x2;
    return p - 1 - small;                /* larger solution */
}
