/*
 * A019383  Primes with primitive root 59.
 * https://oeis.org/A019383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p for which 59 is a primitive root modulo
 *            p. * * Implementation: enumerates primes in increasing order;
 *            the primitive‑root * property is verified by checking that for
 *            every prime divisor q of p–1, * 59^((p–1)/q) mod p ≠ 1. The
 *            prime 59 itself is excluded because 59 ≡ 0 * (mo...
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes.h"  /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/mod_pow.h"       /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static idx_t *prime_array = NULL;         /* primes up to the current sieve limit */
static size_t prime_array_len = 0;
static idx_t  sieve_limit = 0;

/* Ensure prime_array contains all primes <= limit. */
static void ensure_prime_array(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    idx_t *new_array = NULL;
    size_t new_len = 0;

    build_primes(limit, &new_array, &new_len);

    if (prime_array != NULL)
        free(prime_array);

    prime_array = new_array;
    prime_array_len = new_len;
    sieve_limit = limit;
}

/* Test if 59 is a primitive root modulo the prime p. */
static bool is_primitive_root_59(idx_t p)
{
    /* 59 itself cannot be a primitive root (59 is not coprime with 59). */
    if (p == 59)
        return false;

    seq_t base = (seq_t)(59 % p);   /* work in the reduced residue system */
    if (base == 0)
        return false;                /* safety guard */

    idx_t phi = p - 1;
    idx_t n   = phi;

    /* Factor phi using the primes we already have. */
    for (size_t i = 0; i < prime_array_len && prime_array[i] * prime_array[i] <= n; ++i) {
        idx_t q = prime_array[i];
        if (n % q == 0) {
            if (mod_pow(base, phi / q, (seq_t)p) == (seq_t)1)
                return false;
            while (n % q == 0)
                n /= q;
        }
    }

    if (n > 1) {                       /* remaining prime factor */
        if (mod_pow(base, phi / n, (seq_t)p) == (seq_t)1)
            return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset is 1; return 0 for n==0 */

    idx_t limit = 100;            /* initial sieve upper bound */
    size_t found = 0;             /* how many terms have been found */
    size_t processed = 0;         /* index up to which primes were examined */

    for (;;) {
        ensure_prime_array(limit);

        for (size_t i = processed; i < prime_array_len; ++i) {
            idx_t p = prime_array[i];
            if (is_primitive_root_59(p)) {
                ++found;
                if (found == n)
                    return (seq_t)p;
            }
        }

        processed = prime_array_len; /* all primes up to 'limit' have been considered */
        limit *= 2;                  /* enlarge the sieve and continue */
    }
}
