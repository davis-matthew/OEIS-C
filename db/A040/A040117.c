/*
 * A040117  Primes congruent to 5 (mod 12). Also primes p such that x^4 = 9
 *          has no solution mod p.
 * https://oeis.org/A040117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 5 (mod 12).  
   Computed by incremental trial division: the n‑th term a(n) is the
   n‑th prime p with p % 12 == 5.  The function builds up the list on‑demand
   and caches results for repeated calls. */

/* Standard headers needed for integer types and memory handling */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ===== Cache structures ===== */
static seq_t *cacheA = NULL;            /* a(n) values (primes ≡5 mod12) */
static size_t cacheALen = 0;            /* number of elements stored */
static size_t cacheACap = 0;            /* allocated capacity of cacheA */

static uint64_t *primesAll = NULL;      /* all primes discovered so far */
static size_t primesAllLen = 0;         /* number of elements stored */
static size_t primesAllCap = 0;         /* allocated capacity of primesAll */

static uint64_t nextCandidate = 2;      /* next integer to test for primality */

/* Expand a dynamic array of seq_t values */
static void add_cacheA(seq_t value) {
    if (cacheALen == cacheACap) {
        size_t newCap = cacheACap ? cacheACap * 2 : 8;
        cacheA = (seq_t *)realloc(cacheA, newCap * sizeof(seq_t));
        cacheACap = newCap;
    }
    cacheA[cacheALen++] = value;
}

/* Expand a dynamic array of uint64_t primes */
static void add_primeAll(uint64_t p) {
    if (primesAllLen == primesAllCap) {
        size_t newCap = primesAllCap ? primesAllCap * 2 : 8;
        primesAll = (uint64_t *)realloc(primesAll, newCap * sizeof(uint64_t));
        primesAllCap = newCap;
    }
    primesAll[primesAllLen++] = p;
}

/* Test whether a number (uint64_t) is prime using trial division
   by all previously found primes. */
static bool is_prime_uint64(uint64_t cand) {
    if (cand < 2) return false;
    for (size_t i = 0; i < primesAllLen; ++i) {
        uint64_t p = primesAll[i];
        if (p > cand / p) break;      /* p*p would overflow or exceed cand */
        if (cand % p == 0) return false;
    }
    return true;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset 0; safe placeholder */

    /* Generate terms until we have at least n elements in the cache. */
    while (cacheALen < n) {
        /* Check if nextCandidate is prime. */
        bool isPrime = false;
        if (nextCandidate == 2) {
            isPrime = true;
        } else {
            isPrime = is_prime_uint64(nextCandidate);
        }

        if (isPrime) {
            /* Store the prime in the full list of primes. */
            add_primeAll(nextCandidate);

            /* If it satisfies the congruence, store it in the output cache. */
            if ((nextCandidate % 12) == 5) {
                /* Convert to seq_t; safe because the value fits in 64 bits. */
                add_cacheA((seq_t)nextCandidate);
            }
        }

        /* Advance the candidate for the next iteration.  Skip even numbers. */
        if (nextCandidate == 2) {
            nextCandidate = 3;   /* next odd after 2 */
        } else {
            nextCandidate += 2;  /* stay on odd numbers only */
        }
    }

    /* Return the cached term */
    return cacheA[n - 1];
}
