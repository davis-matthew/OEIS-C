/*
 * A040993  Primes p such that x^6 = 2 has no solution mod p.
 * https://oeis.org/A040993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, realloc_or_die
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A040993:
   a(n) is the nth prime p such that the congruence x^6 ≡ 2 (mod p) has no solution. */

/* The test is equivalent to checking whether 2^((p-1)/gcd(6,p-1)) ≠ 1 (mod p). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/mod_pow.h"
#include "../utils/realloc_or_die.h"

static void ensure_capacity(void **p, size_t *cap, size_t need, size_t elem)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 8 : *cap;
        while (newcap < need) newcap <<= 1;
        *p = realloc_or_die(*p, newcap * elem);
        *cap = newcap;
    }
}

static uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* offset is 1, undefined for n==0 */

    /* Static storage for generated primes and sequence values */
    static uint64_t *all_primes = NULL;             /* all primes found (odd ones only) */
    static size_t primes_len  = 0;
    static size_t primes_cap  = 0;

    static seq_t *seq_vals   = NULL;                /* primes satisfying the property */
    static size_t seq_len    = 0;
    static size_t seq_cap    = 0;

    /* grow sequence until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find the next prime candidate */
        uint64_t cand;
        if (primes_len == 0) {
            cand = 3;
        } else {
            cand = all_primes[primes_len - 1] + 2;  /* skip even numbers */
        }

        /* search for a prime */
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t p = all_primes[i];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = false; break; }
            }
            if (is_prime) break;               /* cand is prime */
            cand += 2;                         /* only odd numbers */
        }

        /* store the prime */
        ensure_capacity((void **)&all_primes, &primes_cap,
                        primes_len + 1, sizeof(uint64_t));
        all_primes[primes_len++] = cand;

        /* test the property: 2 has no 6th‑power residue modulo cand */
        uint64_t d = ugcd(6ULL, cand - 1ULL);
        uint64_t e = (cand - 1ULL) / d;
        seq_t res = mod_pow((seq_t)2, e, (seq_t)cand);

        if (res != (seq_t)1) {               /* no solution, include this prime */
            ensure_capacity((void **)&seq_vals, &seq_cap,
                            seq_len + 1, sizeof(seq_t));
            seq_vals[seq_len++] = (seq_t)cand;
        }
    }

    return seq_vals[n - 1];                     /* seq_vals holds the terms in order */
}
