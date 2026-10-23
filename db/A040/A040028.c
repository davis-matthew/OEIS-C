/*
 * A040028  Primes p such that x^3 = 2 has a solution mod p.
 * https://oeis.org/A040028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS sequence A040028.
   A prime p has a solution to x^3 ≡ 2 (mod p) iff
   p = 2, p = 3, p ≡ 2 (mod 3), or p ≡ 1 (mod 3) and 2 is a cubic
   residue modulo p.  The function enumerates primes until it
   has found the required number of such primes. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Storage for the sequence terms. */
static seq_t *terms    = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;

/* Storage for primes discovered during the search. */
static uint64_t *primes   = NULL;
static size_t primes_cnt  = 0;
static size_t primes_cap  = 0;

/* The next integer to test for primality.  Persisted across calls. */
static uint64_t candidate = 2;

/* Ensure that the terms array has at least 'need' capacity. */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap < need) {
        size_t newcap = terms_cap * 2 + 10;
        if (newcap < need) newcap = need;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        terms_cap = newcap;
    }
}

/* Ensure that the primes array has at least 'need' capacity. */
static void ensure_primes_cap(size_t need)
{
    if (primes_cap < need) {
        size_t newcap = primes_cap * 2 + 10;
        if (newcap < need) newcap = need;
        primes = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        primes_cap = newcap;
    }
}

/* Very cheap primality test using the primes discovered so far. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t q = primes[i];
        if (q * q > n) break;
        if (n % q == 0) return false;
    }
    return true;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)  /* the sequence is 1‑indexed; return 0 for safety */
        return (seq_t)0;

    size_t need = (size_t)n;

    /* Generate more terms if required. */
    while (terms_cnt < need) {
        /* Find the next prime starting from 'candidate'. */
        uint64_t p = candidate;
        while (!is_prime_uint64(p))
            ++p;

        /* Append the prime to the master prime list. */
        ensure_primes_cap(primes_cnt + 1);
        primes[primes_cnt++] = p;

        /* Test the cubic‑residue property. */
        bool ok;
        if (p == 2 || p == 3)
            ok = true;
        else if (p % 3 == 2)
            ok = true;
        else {
            seq_t r = mod_pow((seq_t)2, (p - 1) / 3, (seq_t)p);
            ok = (r == (seq_t)1);
        }

        if (ok) {
            ensure_terms_cap(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)p;
        }

        /* Advance candidate for the next search. */
        candidate = p + 1;
    }

    return terms[n - 1];
}
