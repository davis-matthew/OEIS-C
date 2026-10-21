/*
 * A226181  Primes p such that p-1 divided by the period of the binary
 *          expansion of 1/p equals 2^x for some nonnegative integer x.
 * https://oeis.org/A226181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th term of OEIS sequence A226181:
   the primes p such that (p-1)/ord_2(p) is a power of two,
   where ord_2(p) is the multiplicative order of 2 modulo p. */

/* The implementation is a memoised sieve-style generator that
   keeps a cache of all found primes. For each new prime it
   checks the required condition by computing the odd part of
   p-1 and the odd part of the multiplicative order of 2 modulo p. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

/* Return true if prime p satisfies the condition */
static bool satisfies(uint64_t p)
{
    if (p < 3) return false;          /* 2 is not included */

    uint64_t p_minus = p - 1;
    uint64_t odd_p = p_minus >> __builtin_ctzll(p_minus);

    /* Compute the multiplicative order of 2 modulo p */
    uint64_t order = p_minus;      /* initial upper bound */
    uint64_t factor = p_minus;     /* factorisation of p-1 */

    for (uint64_t q = 2; q * q <= factor; ++q) {
        if (factor % q == 0) {
            /* q is a prime divisor of p-1 */
            while (order % q == 0) {
                uint64_t cand = order / q;
                if (mod_pow((seq_t)2, cand, (seq_t)p) == 1)
                    order = cand;
                else
                    break;
            }
            while (factor % q == 0) factor /= q;
        }
    }
    if (factor > 1) {  /* remaining prime factor */
        uint64_t q = factor;
        while (order % q == 0) {
            uint64_t cand = order / q;
            if (mod_pow((seq_t)2, cand, (seq_t)p) == 1)
                order = cand;
            else
                break;
        }
    }

    uint64_t odd_order = order >> __builtin_ctzll(order);
    return odd_order == odd_p;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* cache of found primes */
    static size_t seq_len = 0;     /* number of cached terms */
    static size_t seq_cap = 0;     /* allocated capacity */
    static uint64_t last_prime = 1;/* last prime tested */

    /* Generate primes until we have at least n terms */
    while (seq_len < n) {
        last_prime = next_prime(last_prime + 1);
        if (satisfies(last_prime)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap > 0 ? seq_cap * 2 : 8;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)last_prime;
        }
    }
    return seq[n - 1];
}
