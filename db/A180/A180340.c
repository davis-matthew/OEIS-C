/*
 * A180340  Numbers with x digits such that the first x multiples are cyclic
 *          permutations of the number, leading 0's omitted (or cyclic
 *          numbers).
 * https://oeis.org/A180340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (10^(p-1) - 1) / p, where p = A001913(n) (p is the n‑th
 *            full‑reptend prime, i.e. the base‑10 primitive‑root prime). This
 *            function computes A180340 using a small sieve of primes and
 *            primitive‑root testing with the provided utilities. It works for
 *            any seq_t supported by the build (8‑ to 128‑...
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/mod_pow.h"

static uint64_t *prime_list   = NULL; /* primes discovered so far        */
static size_t    prime_cnt    = 0;
static size_t    prime_cap    = 0;

static uint64_t *reptend_list = NULL; /* A001913(n) = full‑reptend primes */
static size_t    reptend_cnt  = 0;
static size_t    reptend_cap  = 0;

static uint64_t candidate = 2; /* next integer to test for primality */

/* Append a prime to the growing prime list. */
static void push_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Append a found full‑reptend prime to the reptend list. */
static void push_reptend(uint64_t p)
{
    if (reptend_cnt == reptend_cap) {
        size_t new_cap = reptend_cap ? reptend_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(reptend_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        reptend_list = tmp;
        reptend_cap  = new_cap;
    }
    reptend_list[reptend_cnt++] = p;
}

/* Simple deterministic primality test using the primes found so far. */
static int is_prime(uint64_t n)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* Test whether p is a full‑reptend prime (10 is a primitive root modulo p). */
static int is_full_reptend(uint64_t p)
{
    /* 2 and 5 are not full‑reptend primes. */
    if (p == 2 || p == 5) return 0;

    uint64_t m     = p - 1;
    uint64_t temp  = m;
    uint64_t factors[64];
    size_t fcnt    = 0;

    /* Factor m using previously found primes. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > temp) break;
        if (temp % q == 0) {
            factors[fcnt++] = q;
            while (temp % q == 0) temp /= q;
        }
    }
    if (temp > 1) {
        if (fcnt < sizeof(factors) / sizeof(*factors))
            factors[fcnt++] = temp;
    }

    /* Primitive‑root test: 10^(m/q) mod p should not be 1 for any prime q | m. */
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t q = factors[i];
        seq_t r = mod_pow((seq_t)10, (uint64_t)(m / q), (seq_t)p);
        if (r == (seq_t)1) return 0;
    }
    return 1;
}

/* Ensure we have computed at least n full‑reptend primes. */
static void ensure_reptend_count(idx_t n)
{
    while ((idx_t)reptend_cnt < n) {
        /* Find next prime. */
        while (!is_prime(candidate)) ++candidate;

        /* candidate is prime. Store it. */
        push_prime(candidate);

        /* Record it if it is a full‑reptend prime. */
        if (is_full_reptend(candidate))
            push_reptend(candidate);

        ++candidate;      /* advance to the next candidate */
    }
}

/* ------------------------------------------------------------------ */
/* Public routine required by the grading harness.                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, safe return */

    ensure_reptend_count(n);

    uint64_t p = reptend_list[n - 1];            /* A001913(n) */
    seq_t pow10 = big_pow((seq_t)10, (seq_t)(p - 1)); /* 10^(p-1) */
    seq_t num   = pow10 - (seq_t)1;              /* 10^(p-1) - 1 */
    seq_t res   = num / (seq_t)p;                /* division by p */
    return res;
}
