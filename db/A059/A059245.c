/*
 * A059245  Primes p such that x^13 = 2 has no solution mod p.
 * https://oeis.org/A059245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A059245: primes p such that x^13 ≡ 2 (mod p) has no solution. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper function prototypes from the utils directory */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* Test whether the congruence x^13 ≡ 2 (mod p) has a solution.
   Returns true if at least one solution exists. */
static bool has_solution_13th_power_2(uint64_t p)
{
    if (p == 2) return true;                     /* 0^13 = 0 ≡ 2 (mod 2) is possible      */
    uint64_t d   = (p % 13 == 1) ? 13 : 1;        /* gcd(13, p-1) */
    uint64_t exp = (p - 1) / d;                   /* exponent for the criterion         */
    seq_t   r    = mod_pow((seq_t)2, exp, (seq_t)p); /* 2^exp mod p */
    return r == (seq_t)1;                        /* solution exists iff r == 1          */
}

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Caching of the sequence values */
    static seq_t  *seq      = NULL;
    static size_t seq_len   = 0;
    static size_t seq_cap   = 0;
    static uint64_t last_prime = 2;
    static bool     initialized = false;

    /* Initialise the cache on the first call */
    if (!initialized) {
        seq_cap   = 256;
        seq       = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len   = 0;
        initialized = true;
    }

    /* Generate primes until we have at least n terms */
    while (seq_len < (size_t)n) {
        uint64_t p = next_prime(last_prime + 1);   /* next prime > last_prime */
        last_prime = p;

        if (!has_solution_13th_power_2(p)) {       /* include if no solution exists */
            if (seq_len == seq_cap) {              /* grow the buffer if needed */
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
