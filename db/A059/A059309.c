/*
 * A059309  Primes p such that x^20 = 2 has no solution mod p.
 * https://oeis.org/A059309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p for which 2 is not a 20‑th power modulo
 *            p, i.e. 2^((p−1)/gcd(20,p−1)) ≠ 1 (mod p)
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates primes on demand, tests each one with the
   above congruence using modular exponentiation, and stores the qualifying
   primes in a static cache.  The cache allows out‑of‑order calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd_u64.h"

/* 64‑bit Euclidean gcd. */

/* Returns the n‑th term of the sequence.  The function keeps a static cache
   of already found terms and a list of all primes generated so far. */
seq_t a(idx_t n)
{
    /* static cache of sequence terms */
    static seq_t  *seq      = NULL;
    static size_t seq_cap   = 0;
    static idx_t  seq_len   = 0;          /* number of terms stored */

    /* list of all primes generated up to now (used for primality tests) */
    static uint64_t *prime_list = NULL;
    static size_t prime_cap  = 0;
    static size_t prime_len  = 0;

    /* generate new qualifying primes until we have at least n terms */
    while (seq_len < n) {
        /* ---- generate the next prime ---- */
        uint64_t candidate = (prime_len == 0) ? 3ULL
                                              : prime_list[prime_len - 1] + 2ULL;
        while (1) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t q = prime_list[i];
                /* stop when q*q > candidate (no overflow for our intended limits) */
                if (q * q > candidate) break;
                if (candidate % q == 0) { is_prime = false; break; }
            }
            if (is_prime) break;          /* candidate is prime */
            candidate += 2ULL;            /* next odd candidate */
        }

        /* store the new prime in the list */
        if (prime_len >= prime_cap) {
            prime_cap = (prime_cap ? prime_cap * 2 : 16);
            uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_list = tmp;
        }
        prime_list[prime_len++] = candidate;
        uint64_t p = candidate;

        /* ---- test the solvability condition for this prime ---- */
        uint64_t g   = gcd_u64(20ULL, p - 1ULL);    /* gcd(20,p-1) */
        uint64_t exp = (p - 1ULL) / g;             /* (p-1)/gcd(20,p-1)  */

        seq_t value = mod_pow((seq_t)2, exp, (seq_t)p);  /* 2^exp mod p */

        if (value != (seq_t)1) {              /* 2 is not a 20‑th power */
            /* store in the sequence cache */
            if (seq_len >= seq_cap) {
                seq_cap = (seq_cap ? seq_cap * 2 : 16);
                seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq = tmp;
            }
            seq[seq_len++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
