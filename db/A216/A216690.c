/*
 * A216690  Primes p such that x^4 = -2 has no solution mod p.
 * https://oeis.org/A216690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that x^4 ≡ -2 (mod p) has no solution. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

/* Test whether a prime p has a solution to x^4 ≡ -2 (mod p). */
static bool has_solution(uint64_t p) {
    if (p == 2 || p == 3) return true;          /* 2 and 3 are excluded */
    seq_t a = (seq_t)(p - 2);                   /* -2 mod p */
    if ((p - 1) % 4 == 0) {                     /* p ≡ 1 (mod 4) */
        uint64_t exp = (p - 1) / 4;
        return mod_pow(a, exp, (seq_t)p) == 1;
    } else {                                    /* p ≡ 3 (mod 4) */
        uint64_t exp = (p - 1) / 2;
        return mod_pow(a, exp, (seq_t)p) == 1;
    }
}

/* Cached values of the sequence. */
static seq_t *vals = NULL;
static size_t val_len = 0, val_cap = 0;
static uint64_t last_prime = 3;      /* primes found so far (2,3 included) */

/* Ensure that vals has at least n terms. */
static void ensure_n_terms(idx_t n) {
    while (val_len < (size_t)n) {
        /* Find the next prime after last_prime. */
        last_prime = next_prime((idx_t)(last_prime + 1));
        if (!has_solution(last_prime)) {
            /* Grow storage if needed. */
            if (val_len == val_cap) {
                val_cap = val_cap ? val_cap * 2 : 16;
                vals = (seq_t *)realloc(vals, val_cap * sizeof(seq_t));
            }
            vals[val_len++] = (seq_t)last_prime;
        }
    }
}

/* Public entry point: a(n). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* out-of-range guard */
    ensure_n_terms(n);
    return vals[n - 1];                     /* a(1) is the first stored element */
}
