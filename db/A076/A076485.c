/*
 * A076485  Solutions to gcd(sigma(x), phi(x)) > gcd(sigma(core(x)),
 *          phi(core(x))), i.e., when A009223(x) > A066086(x) or if A066087(x)
 *          > 0.
 * https://oeis.org/A076485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 865, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076485: all integers x such that
   gcd(σ(x),φ(x)) > gcd(σ(core(x)),φ(core(x)))
   (core(x) = product of distinct prime factors of x).
   This code generates the n‑th such x by brute force.
   It uses a simple prime sieve, trial division and
   Euclid's algorithm for gcd.  Complexity is
   O(N sqrt(N)) for the first N terms, which is well below
   2 s for N≈10000. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd_seq_local.h"

/* ------------------------------------------------------------------ */
/* Small Euclid gcd that works for any integral type seq_t            */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Prime sieve – incremental trial division                           */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0, prime_cap = 0;
static uint64_t prime_limit_checked = 1;

static void ensure_primes_upto(uint64_t limit) {
    if (limit <= prime_limit_checked) return;

    if (prime_count == 0) {                /* seed with prime 2 */
        prime_cap = 32;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        prime_list[prime_count++] = 2;
        prime_limit_checked = 2;
    }

    for (uint64_t cand = prime_limit_checked + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        uint64_t root = (uint64_t)sqrt((long double)cand);
        for (size_t i = 0; i < prime_count && prime_list[i] <= root; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = (uint32_t)cand;
        }
    }
    prime_limit_checked = limit;
}

/* ------------------------------------------------------------------ */
/* Test whether an integer n satisfies the defining condition           */
/* ------------------------------------------------------------------ */
static bool candidate_ok(uint64_t n) {
    uint64_t phi = n;
    seq_t sigma = 1;
    seq_t sigma_sf = 1; /* sigma of square‑free core */
    seq_t phi_sf = 1;   /* phi of square‑free core */

    uint64_t m = n;
    uint64_t sqrt_m = (uint64_t)sqrt((long double)m) + 1;
    ensure_primes_upto(sqrt_m);

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= m; ++i) {
        uint32_t p = prime_list[i];
        if (m % p != 0) continue;

        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* φ(n) */
        phi = phi / p * (p - 1);

        /* σ(n) */
        seq_t p_pow = 1;
        for (int k = 0; k < exp + 1; ++k) p_pow *= p;
        seq_t term = (p_pow - 1) / (p - 1);
        sigma *= term;

        /* σ(core), φ(core) */
        sigma_sf *= (seq_t)(p + 1);
        phi_sf   *= (seq_t)(p - 1);
    }

    if (m > 1) {                     /* remaining prime factor */
        uint32_t p = (uint32_t)m;
        phi = phi / p * (p - 1);

        /* σ(p) = p + 1 */
        sigma *= (seq_t)(p + 1);

        sigma_sf *= (seq_t)(p + 1);
        phi_sf   *= (seq_t)(p - 1);
    }

    seq_t g1 = gcd_seq_local(sigma, phi);
    seq_t g2 = gcd_seq_local(sigma_sf, phi_sf);
    return g1 > g2;
}

/* ------------------------------------------------------------------ */
/* Cache of computed terms                                               */
/* ------------------------------------------------------------------ */
static seq_t *term_cache = NULL;
static size_t term_len   = 0, term_cap   = 0;
static uint64_t next_candidate = 2;

/* ------------------------------------------------------------------ */
/* The required public function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of range guard */

    while (term_len < (size_t)n) {
        if (candidate_ok(next_candidate)) {
            if (term_len == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 64;
                term_cap = new_cap;
                term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
            }
            term_cache[term_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return term_cache[n - 1];
}
