/*
 * A216739  Primes p such that x^16 = -2 has no solution mod p.
 * https://oeis.org/A216739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cap_uint64, gcd_uint64, grow_seq_array, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A216739: primes p such that x^16 = -2 has no solution mod p.
   The function searches successive primes and tests the solvability via
   Euler's criterion:  x^16 = -2 has a solution iff (-2)^((p-1)/gcd(16,p-1)) ≡ 1 (mod p).  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/grow_seq_array.h"
#include "../utils/ensure_cap_uint64.h"
#include "../utils/gcd_uint64.h"

/* static state */
static uint64_t *prime_list = NULL;
static size_t prime_len = 0, prime_cap = 0;

static seq_t *cand_list = NULL;
static size_t cand_len = 0, cand_cap = 0;

static uint64_t next_candidate = 5;          /* next odd integer to test for primality */

/* GCD for uint64_t */

/* Test primality using the current prime list (trial division). */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Append a prime to the prime list. */
static void append_prime(uint64_t p)
{
    ensure_cap_uint64(&prime_cap, prime_len + 1, &prime_list);
    prime_list[prime_len++] = p;
}

/* Append a candidate prime to the sequence list. */
static void append_candidate(uint64_t p)
{
    if (cand_len == cand_cap) {
        size_t new_cap = cand_cap ? (cand_cap << 1) : 16;
        grow_seq_array(&cand_list, &cand_cap, new_cap);
    }
    cand_list[cand_len++] = (seq_t)p;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0 */

    /* Initialise prime list with the first two primes once. */
    if (prime_len == 0) {
        append_prime(2);
        append_prime(3);
    }

    /* Build the sequence until we have at least n terms. */
    while (cand_len < (size_t)n) {
        /* Find the next prime. */
        uint64_t p;
        for (;;) {
            if (is_prime_candidate(next_candidate)) {
                p = next_candidate;
                append_prime(p);
                next_candidate += 2;   /* skip even numbers */
                break;
            }
            next_candidate += 2;
        }
        if (p == 2) continue;  /* 2 is excluded by definition */

        /* Compute whether -2 is a 16‑th power modulo p. */
        uint64_t d   = gcd_uint64(16, p - 1);
        uint64_t exp = (p - 1) / d;

        seq_t y   = (seq_t)p - 2;    /* -2 mod p */
        seq_t mod = (seq_t)p;
        seq_t r = mod_pow(y, exp, mod);

        if (r != (seq_t)1)          /* unsolvable → include in sequence */
            append_candidate(p);
    }

    return cand_list[n - 1];
}
