/*
 * A216734  Primes p such that x^6 = -2 has no solution mod p.
 * https://oeis.org/A216734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216734: primes p for which the congruence x^6 = -2 (mod p) has no solution. */
/* The function a(idx_t n) returns the nth such prime (offset 1). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* 64‑bit GCD. */

/* 64‑bit modular exponentiation using 128‑bit multiplication to avoid overflow. */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Dynamic array for primes (uint64_t). */
static uint64_t *prime_arr = NULL;
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;

/* Dynamic array for terms (seq_t). */
static seq_t *term_arr = NULL;
static size_t term_cnt  = 0;
static size_t term_cap  = 0;

/* Ensure the prime array has space for one more element. */
static void ensure_prime_cap(void)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cnt == prime_cap) {
        prime_cap *= 2;
        prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
    }
}

/* Ensure the term array has space for one more element. */
static void ensure_term_cap(void)
{
    if (term_cap == 0) {
        term_cap = 16;
        term_arr = (seq_t *)malloc(term_cap * sizeof(seq_t));
    } else if (term_cnt == term_cap) {
        term_cap *= 2;
        term_arr = (seq_t *)realloc(term_arr, term_cap * sizeof(seq_t));
    }
}

/* Trial division primality test using the primes already found. */
static inline bool is_prime_candidate(uint64_t n)
{
    if (n <= 1ULL)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if ((unsigned __int128)p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Generate terms until we have at least n of them. */
static void ensure_terms(idx_t n)
{
    static uint64_t next_candidate = 2ULL;   /* Next integer to test for primality. */

    while (term_cnt < (size_t)n) {
        /* Find the next prime. */
        while (!is_prime_candidate(next_candidate)) {
            next_candidate += (next_candidate == 2ULL) ? 1ULL : 2ULL;
        }

        /* Store the new prime. */
        ensure_prime_cap();
        prime_arr[prime_cnt++] = next_candidate;

        /* Check the 6th‑power residue condition. */
        if (next_candidate >= 5ULL) {
            uint64_t p = next_candidate;
            uint64_t g = gcd64(6ULL, p - 1ULL);
            uint64_t exp = (p - 1ULL) / g;
            uint64_t base = (p >= 2ULL) ? (p - 2ULL) : 0ULL;
            uint64_t val = mod_pow_u64(base, exp, p);
            if (val != 1ULL) {             /* No solution, so include. */
                ensure_term_cap();
                term_arr[term_cnt++] = (seq_t)p;
            }
        }

        /* Advance to the next integer (skip even numbers). */
        next_candidate += (next_candidate == 2ULL) ? 1ULL : 2ULL;
    }
}

/* External function requested by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* Undefined for n=0; return 0 as a safe dummy. */
    ensure_terms(n);
    return term_arr[n - 1];
}
