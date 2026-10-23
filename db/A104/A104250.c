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
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   isPrimeUint64, sumPrimeDigits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeUint64.h"
#include "../utils/sumPrimeDigits.h"

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
        if (isPrimeUint64(cand))
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
