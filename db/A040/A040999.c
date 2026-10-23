/*
 * A040999  Primes p such that x^6 = 5 has no solution mod p.
 * https://oeis.org/A040999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that the congruence x^6 ≡ 5 (mod p) has no
 *            solution
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental prime generation, testing 5^( (p‑1)/gcd(6,p‑1) ) mod p. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"   /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/is_prime.h" /* bool is_prime(idx_t x) */

/* static storage for the generated part of the sequence */
static seq_t *seq = NULL;       /* array where terms are stored          */
static size_t seq_len = 0;      /* number of terms already stored        */
static size_t seq_cap = 0;      /* current capacity of seq               */
static uint64_t next_candidate = 2; /* next integer to be tested for primality */

/* Euclidean GCD for uint64_t */
static uint64_t u64_gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Grow the seq array to hold at least need elements.  Uses realloc. */
static void ensure_seq_cap(size_t need) {
    (void)need; /* need is implicitly handled */
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap == 0 ? 128 : seq_cap * 2;
        /* realloc must not cast in C++ */
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* unlikely to happen with small limits */
            exit(1);
        }
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* undefined for offset = 1 */

    while (seq_len < (size_t)n) {
        /* find the next prime */
        uint64_t p = next_candidate;
        while (!is_prime(p)) ++p;
        next_candidate = p + 1;    /* start after this prime on next round */

        /* skip p = 5 – trivial solution x ≡ 0 (mod 5) */
        if (p == 5ULL)
            continue;

        uint64_t d   = u64_gcd(6ULL, p - 1ULL);
        uint64_t exp = (p - 1ULL) / d;
        seq_t r = mod_pow(5ULL, exp, p);

        if (r != 1) {              /* 5 is NOT a sixth‑power residue */
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
