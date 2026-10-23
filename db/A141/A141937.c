/*
 * A141937  Primes congruent to 13 mod 25.
 * https://oeis.org/A141937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime such that p ≡ 13 (mod 25)
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *primesAll = NULL;   /* all primes found so far */
static size_t primesSize = 0, primesCap = 0;

static seq_t *matches   = NULL;   /* primes ≡ 13 mod 25 */
static size_t matchesSize = 0, matchesCap = 0;

static idx_t nextPrimeCheck = 2;   /* next integer to test for primality */
static idx_t candidateK = 0;       /* k in 25*k+13 */

/* Ensure static arrays are initialized */
static void a_init(void)
{
    primesCap = 16;
    primesAll  = (seq_t *)calloc(primesCap, sizeof(seq_t));
    matchesCap = 16;
    matches    = (seq_t *)calloc(matchesCap, sizeof(seq_t));

    /* prime 2 */
    primesAll[0] = (seq_t)2;
    primesSize = 1;
    nextPrimeCheck = 3;      /* next number to test */
    candidateK = 0;
}

/* Add a prime to the list of all primes, growing the array as needed */
static void add_prime_all(seq_t p)
{
    if (primesSize == primesCap) {
        primesCap <<= 1;
        primesAll = (seq_t *)realloc(primesAll, primesCap * sizeof(seq_t));
    }
    primesAll[primesSize++] = p;
}

/* Add a prime to the matches list, growing the array as needed */
static void add_match(seq_t p)
{
    if (matchesSize == matchesCap) {
        matchesCap <<= 1;
        matches = (seq_t *)realloc(matches, matchesCap * sizeof(seq_t));
    }
    matches[matchesSize++] = p;
}

/* Return true if n is prime using the current prime list */
static bool is_prime_trial(idx_t n)
{
    for (size_t i = 0; i < primesSize; ++i) {
        seq_t p = primesAll[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure we have all primes up to sqrt(limit) in primesAll */
static void build_primes_up_to(seq_t limit)
{
    seq_t sqrtLimit = floor_sqrt(limit);
    idx_t sqrtIdx = (idx_t)sqrtLimit;

    while (nextPrimeCheck <= sqrtIdx) {
        if (is_prime_trial(nextPrimeCheck)) {
            add_prime_all((seq_t)nextPrimeCheck);
        }
        ++nextPrimeCheck;
    }
}

/* Check if candidate is prime using the full prime list */
static bool is_candidate_prime(seq_t candidate)
{
    for (size_t i = 0; i < primesSize; ++i) {
        seq_t p = primesAll[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (!primesAll) a_init();

    while (matchesSize < n) {
        /* Generate next candidate 25*k+13 */
        seq_t candidate = (seq_t)25 * (seq_t)candidateK + 13;
        ++candidateK;

        /* Ensure primes up to sqrt(candidate) are known */
        build_primes_up_to(candidate);

        if (is_candidate_prime(candidate)) {
            add_match(candidate);
        }
    }
    return matches[n - 1];
}
