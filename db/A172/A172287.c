/*
 * A172287  Primes p such that exactly one of 2p-3 and 3p-2 is prime.
 * https://oeis.org/A172287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime p such that exactly one of 2p-3 and 3p-2 is prime. */
/* The function builds the sequence lazily with memoisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------------------------------------------------------------------- */
/* Static storage for the sequence and the list of known primes.           */
/* ---------------------------------------------------------------------- */
static uint64_t nextPrimeCandidate = 2;          /* next prime to test */

static uint64_t *primeList = NULL;              /* array of primes ≤ current candidate */
static size_t primeCount = 0;                   /* number of primes stored */
static size_t primeCap   = 0;                   /* allocated capacity for primeList */

static seq_t *termList = NULL;                  /* cached sequence terms */
static size_t termCount  = 0;                   /* number of terms cached */
static size_t termCap    = 0;                   /* allocated capacity for termList */

/* ---------------------------------------------------------------------- */
/* Helper to grow the prime array.                                         */
/* ---------------------------------------------------------------------- */
static void ensurePrimeCap(void) {
    if (primeCount >= primeCap) {
        size_t newCap = primeCap ? primeCap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primeList, newCap * sizeof(uint64_t));
        primeList = tmp;
        primeCap = newCap;
    }
}

/* ---------------------------------------------------------------------- */
/* Helper to grow the term array.                                          */
/* ---------------------------------------------------------------------- */
static void ensureTermCap(void) {
    if (termCount >= termCap) {
        size_t newCap = termCap ? termCap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(termList, newCap * sizeof(seq_t));
        termList = tmp;
        termCap = newCap;
    }
}

/* ---------------------------------------------------------------------- */
/* Trial‑division primality test using the primes already known.  The
   list always contains all primes < current candidate, so it is
   sufficient for any test performed for values ≤ current candidate^2. */
static bool is_prime_u64(uint64_t n) {
    if (n < 2)          return false;
    if (n == 2)         return true;
    if ((n & 1) == 0)   return false;

    for (size_t i = 0; i < primeCount; ++i) {
        uint64_t p = primeList[i];
        if (p > n / p)   break;          /* avoid overflow */
        if (n % p == 0)  return false;
    }
    return true;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function.                                                */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Return cached value if available */
    if (n <= termCount) {
        return termList[n - 1];
    }

    /* Generate further terms until we have at least n of them.    */
    while (termCount < (size_t)n) {
        uint64_t p;

        /* Find the next prime. */
        for (;;) {
            if (nextPrimeCandidate < 2) {
                nextPrimeCandidate = 2;
            }
            if (nextPrimeCandidate == 2) {
                p = 2;
                nextPrimeCandidate = 3;
            } else {
                p = nextPrimeCandidate;
                nextPrimeCandidate += 2;  /* only odd candidates after 2 */
            }

            if (is_prime_u64(p)) {
                /* Store the new prime. */
                ensurePrimeCap();
                primeList[primeCount++] = p;

                /* Check the defining condition. */
                uint64_t v1 = 2 * p - 3;
                uint64_t v2 = 3 * p - 2;
                int primesBelow = 0;
                primesBelow += is_prime_u64(v1);
                primesBelow += is_prime_u64(v2);

                if (primesBelow == 1) {
                    /* Store the term. */
                    ensureTermCap();
                    termList[termCount++] = (seq_t)p;
                    /* If we have enough terms, exit the outer loop. */
                    if (termCount >= (size_t)n) {
                        break;
                    }
                }
                break;   /* move to the next prime candidate */
            }
        }
    }

    return termList[n - 1];
}
