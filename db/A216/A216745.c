/*
 * A216745  Primes p such that x^28 = -2 has no solution mod p.
 * https://oeis.org/A216745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p (in increasing order) such that the
 *            congruence x^28 ≡ –2 (mod p) has no solution. The function
 *            computes the sequence by iterating over primes and testing the
 *            solvability criterion
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=====================================================================*/
/*  NOTE: The code assumes the build supplies the following utilities: */
/*        - mod_pow(seq_t base, uint64_t exp, seq_t mod) */
/*=====================================================================*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* ------------------------------------------------------------------ */
/*  Helper: GCD for 64‑bit unsigned integers                          */
/* ------------------------------------------------------------------ */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Prime list used for trial division                                 */
/* ------------------------------------------------------------------ */
static uint64_t *primeList = NULL;
static size_t    primeCap   = 0;
static size_t    primeCnt   = 0;

static void add_prime(uint64_t p)
{
    if (primeCnt == primeCap) {
        size_t newCap = primeCap ? primeCap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(primeList, newCap * sizeof(uint64_t));
        if (!tmp) exit(1);
        primeList = tmp;
        primeCap  = newCap;
    }
    primeList[primeCnt++] = p;
}

static bool is_candidate_prime(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primeCnt; ++i) {
        uint64_t p = primeList[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Sequence cache (array of seq_t values)                            */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;
static size_t cacheCap   = 0;
static size_t cacheSize  = 0;

static void add_cache(seq_t val)
{
    if (cacheSize == cacheCap) {
        size_t newCap = cacheCap ? cacheCap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cacheCap = newCap;
    }
    cache[cacheSize++] = val;
}

/* ------------------------------------------------------------------ */
/*  Static state keeping the last candidate examined                   */
/* ------------------------------------------------------------------ */
static uint64_t currentCandidate = 3;  /* start checking from 3 */

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise static structures on first call */
    if (primeCnt == 0) {
        add_prime(2);                /* prime list starts with 2 */
        currentCandidate = 3;         /* start search from 3 */
    }

    /* Generate primes until we have at least n elements in the cache */
    while (cacheSize < (size_t)n) {

        /* Find the next prime candidate after currentCandidate-1 */
        while (!is_candidate_prime(currentCandidate))
            ++currentCandidate;

        /* currentCandidate is prime */
        add_prime(currentCandidate);

        /* Test the solvability criterion; skip p = 2 (handled above) */
        if (currentCandidate > 2) {
            uint64_t p   = currentCandidate;
            uint64_t d   = gcd_u64(28, p - 1);
            uint64_t exp = (p - 1) / d;

            seq_t base  = (seq_t)p - (seq_t)2; /* -2 mod p */
            seq_t power = mod_pow(base, exp, (seq_t)p);

            if (power != (seq_t)1)     /* no solution, so record p */
                add_cache((seq_t)p);
        }

        /* Advance to next integer for the next loop iteration */
        ++currentCandidate;
    }

    /* Return the cached value (1‑based index) */
    return cache[n - 1];
}
