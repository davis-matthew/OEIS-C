/*
 * A073684  Sum of next a(n) successive primes is prime.
 * https://oeis.org/A073684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Group consecutive primes so that each group's sum is a prime.
   The first group is (2, 3) and has length 2. From the second group on,
   each group must contain at least three primes.  a(n) is the length of
   the nth group (offset 1).  The function builds groups lazily and
   memoises the lengths. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

#define MAX_GROUPS 20000   /* enough for the first 10 000 terms */
#define MAX_PRIMES 200000 /* generous upper bound for primes needed */

static seq_t  groupLens[MAX_GROUPS];
static size_t groupCount = 0;

static idx_t  primes[MAX_PRIMES];
static size_t primeCount = 0;
static idx_t  candidate = 0;
static bool   primesInited = false;

/* initialise the prime list with 2 and 3, and set the next candidate */
static void initPrimes(void)
{
    if (!primesInited) {
        primes[0] = 2;
        primes[1] = 3;
        primeCount = 2;
        candidate = 5;                /* next odd to test after 3 */
        primesInited = true;
    }
}

/* return the next prime greater than the last stored prime */
static idx_t nextPrime(void)
{
    initPrimes();                     /* guarantee primes array ready */
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < primeCount; ++i) {
            const idx_t p = primes[i];
            if (p * p > candidate) break;   /* only need divisors ≤ sqrt(candidate) */
            if (candidate % p == 0) { ok = false; break; }
        }
        if (ok) {
            if (primeCount >= MAX_PRIMES) return candidate; /* safety guard */
            primes[primeCount++] = candidate;
            const idx_t ret = candidate;
            candidate += 2;          /* skip even numbers */
            return ret;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 */

    /* if the requested group is already computed, return it */
    if (groupCount >= (size_t)n) return groupLens[(size_t)n-1];

    /* group 1 is a special case: (2,3) */
    if (groupCount == 0) {
        groupLens[0] = (seq_t)2;
        groupCount = 1;
    }

    /* compute subsequent groups lazily */
    while (groupCount < (size_t)n) {
        idx_t sum  = 0;
        idx_t len  = 0;
        const idx_t minLen = 3;          /* from the second group onward */

        for (;;) {
            const idx_t p = nextPrime();
            sum += p;
            ++len;
            if (len >= minLen && isPrime(sum)) {
                groupLens[groupCount] = (seq_t)len;
                ++groupCount;
                break;
            }
        }
    }
    return groupLens[(size_t)n-1];
}
