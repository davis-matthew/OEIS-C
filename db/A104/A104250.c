/*
 * A104250  Sum of prime digits of n-th prime.
 * https://oeis.org/A104250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of prime digits (2,3,5,7) of the n‑th prime
 *            symbolically (OEIS A104250). Implementation uses memoised prime
 *            generation; for each n the nth prime is computed once, then its
 *            prime digits are summed
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 113 terms through n = 8745 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Sum the prime digits 2,3,5,7 of a 64‑bit integer.  The sum always
   fits in 64 bits for the primes up to n ≈ 10 000.                     */
static uint64_t sumPrimeDigits(uint64_t x)
{
    uint64_t sum = 0;
    while (x)
    {
        uint8_t d = (uint8_t)(x % 10);
        if (d == 2 || d == 3 || d == 5 || d == 7)
            sum += d;
        x /= 10;
    }
    return sum;
}

/* ---------- internal static helpers ------------------------------------- */

static uint64_t *primes = NULL;   /* dynamic array of found primes          */
static size_t primeCnt = 0;       /* how many primes stored                */
static size_t primeCap = 0;       /* allocated capacity                    */

/* Expand the prime array if necessary and store a new prime */
static void addPrime(uint64_t p)
{
    if (primeCnt == primeCap)
    {
        size_t newCap = primeCap ? primeCap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, newCap * sizeof(uint64_t));
        if (!tmp) exit(1);            /* allocation failed */
        primes = tmp;
        primeCap = newCap;
    }
    primes[primeCnt++] = p;
}

/* Return the nth prime (1‑based).  Memoises results so that repeated
   calls are fast.                                    */
static uint64_t nthPrime(idx_t n)
{
    if (primeCnt == 0)          /* initialise with first two primes */
    {
        addPrime(2);
        addPrime(3);
    }

    /* generate more primes if we haven't reached nth yet */
    uint64_t cand = primes[primeCnt - 1] + 1;
    while (primeCnt < n)
    {
        if (is_prime_u64(cand))
            addPrime(cand);
        cand++;
    }
    return primes[n - 1];
}

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* values below offset are undefined */
    uint64_t p   = nthPrime(n);
    uint64_t sum = sumPrimeDigits(p);
    return (seq_t)sum;               /* cast to seq_t (may be arbitrary‑precision) */
}
