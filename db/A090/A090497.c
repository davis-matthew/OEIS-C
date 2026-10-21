/*
 * A090497  Smallest prime p such that the concatenation 2,3,5,7, ... (primes)
 *          ... p is a multiple of prime(n).
 * https://oeis.org/A090497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 303, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A090497: */
/* a(n) is the smallest prime p such that the decimal concatenation of all primes up to p is divisible by prime(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_uint64.h"

/* Internal static storage of primes for incremental generation */
static uint64_t *primes = NULL;     /* array of primes, 1‑based in logic but 0‑indexed in practice */
static size_t primes_cnt = 0;       /* number of primes stored */
static size_t primes_cap = 0;       /* current capacity of the array */

/* Simple incremental prime generator based on trial division. */
static void ensure_primes(size_t need)
{
    if (need <= primes_cnt) return;
    /* initial capacity */
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    }
    uint64_t candidate;
    while (primes_cnt < need) {
        /* determine next candidate */
        if (primes_cnt == 0)
            candidate = 2;
        else
            candidate = primes[primes_cnt - 1] + 1;

        /* find the next prime */
        while (true) {
            bool isprime = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t d = primes[i];
                if (d * d > candidate) break;
                if (candidate % d == 0) { isprime = false; break; }
            }
            if (isprime) break;
            ++candidate;
        }
        /* store the prime */
        if (primes_cnt == primes_cap) {
            primes_cap *= 2;
            primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_cnt++] = candidate;
    }
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but not expected */

    /* Make sure we have at least n primes for modulus and searching. */
    ensure_primes((size_t)n);

    /* Modulus is the n-th prime (1‑based). */
    uint64_t m = primes[(size_t)n - 1];
    uint64_t cur = 0;                 /* current concatenation modulo m */

    for (size_t idx = 0;; ++idx) {
        /* ensure the prime list contains the current candidate */
        if (idx == primes_cnt) ensure_primes(primes_cnt + 1);

        uint64_t p = primes[idx];
        unsigned len = digits_uint64(p);

        /* compute 10^len mod m */
        uint64_t pow10 = 1 % m;
        for (unsigned i = 0; i < len; ++i)
            pow10 = (pow10 * 10) % m;

        /* update the running concatenation modulo m */
        cur = (cur * pow10 + p % m) % m;

        if (cur == 0)
            return (seq_t)p;
    }
}
