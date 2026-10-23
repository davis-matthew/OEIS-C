/*
 * A167804  Numbers with primitive root -8.
 * https://oeis.org/A167804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computes the nth number n (n ≥ 1) such that –8 is a
 *            primitive root modulo n. In other words, -8 has multiplicative
 *            order φ(n) modulo n. Such n must be odd and of the form p^k with
 *            an odd prime p
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Return true if -8 is a primitive root modulo cand. */
static bool has_primitive_neg8(uint64_t cand)
{
    /* n must be odd; otherwise gcd(n,8) ≠ 1. */
    if ((cand & 1ULL) == 0)
        return false;

    /* Check that cand is a prime power p^k, p odd. */
    uint64_t tmp = cand;
    uint64_t p_factor = 0;          /* a prime divisor of cand */
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            p_factor = d;
            break;
        }
    }
    if (p_factor == 0)                /* cand is prime itself */
        p_factor = tmp;

    /* Remove all occurrences of this prime factor. */
    uint64_t remaining = tmp;
    while (remaining % p_factor == 0)
        remaining /= p_factor;
    if (remaining != 1)                  /* more than one distinct prime factor */
        return false;

    /* Euler totient: φ(cand) = cand – cand/p_factor. */
    uint64_t phi = cand - cand / p_factor;

    /* Compute base = (-8) mod cand, in the range [0, cand-1]. */
    long long base_ll = (long long)cand - 8;
    if (base_ll < 0)
        base_ll += cand;
    seq_t base = (seq_t)base_ll;

    /* Determine the multiplicative order of base modulo cand. */
    uint64_t ord = phi;
    uint64_t t = phi;

    /* Factor φ and attempt to reduce the order. */
    for (uint64_t d = 2; d * d <= t; d += (d == 2 ? 1 : 2)) {
        if (t % d == 0) {
            while (ord % d == 0 && mod_pow(base, (uint64_t)(ord / d), (seq_t)cand) == 1)
                ord /= d;
            while (t % d == 0)
                t /= d;
        }
    }
    if (t > 1) {                          /* last prime factor of φ */
        while (ord % t == 0 && mod_pow(base, (uint64_t)(ord / t), (seq_t)cand) == 1)
            ord /= t;
    }

    return ord == phi;                     /* true iff -8 is primitive root. */
}

/* Return the nth term of the sequence (a(1) is first). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_candidate = 5;   /* start from the first odd candidate */

    while (terms_len < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;                 /* skip even numbers */

        if (has_primitive_neg8(cand)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)cand;
        }
    }
    return terms[n - 1];
}
