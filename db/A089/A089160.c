/*
 * A089160  Numbers k such that 30*k + 11 and 30*k + 13 are twin primes.
 * https://oeis.org/A089160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that 30*k+11 and 30*k+13 are twin primes. Computes
 *            by sieving primes on demand and keeping a table of found k
 * Range:     u8 n <= 58, u16 n <= 4878, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* static state */
static uint8_t *isPrime = NULL;   /* bitmap of primes up to current limit */
static size_t   sieveLimit = 0;   /* current sieve size (last index) */

static idx_t *kList = NULL;   /* list of found k values */
static size_t  kCount   = 0;  /* number of k values stored */
static size_t  kCapacity = 0; /* allocated capacity of kList */
static idx_t   nextK     = 0;  /* next k to test */

/* initialise sieve up to 'limit' (inclusive) */
static void init_sieve(size_t limit)
{
    if (limit <= sieveLimit)          /* already large enough */
        return;

    free(isPrime);
    isPrime = (uint8_t*)malloc(limit + 1);
    if (!isPrime) exit(1);            /* out of memory */

    /* assume all are prime */
    for (size_t i = 0; i <= limit; ++i) isPrime[i] = 1;

    isPrime[0] = isPrime[1] = 0;
    size_t r = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= r; ++i)
    {
        if (isPrime[i])
        {
            for (size_t j = i * i; j <= limit; j += i)
                isPrime[j] = 0;
        }
    }
    sieveLimit = limit;
}

/* ensure that the sieve covers at least up to 'limit' */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieveLimit) return;

    /* grow buffer more than needed to avoid frequent reallocations */
    size_t newLimit = limit * 2 + 1000;
    if (newLimit < limit)            /* overflow guard */
        newLimit = (size_t)-1;
    init_sieve(newLimit);
}

/* add a new k to the list, expanding storage if needed */
static void store_k(idx_t k)
{
    if (kCount == kCapacity)
    {
        size_t newCap = kCapacity ? kCapacity * 2 : 16;
        idx_t *tmp = (idx_t*)realloc(kList, newCap * sizeof(idx_t));
        if (!tmp) exit(1);
        kList = tmp;
        kCapacity = newCap;
    }
    kList[kCount++] = k;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: offset is 1, but guard against 0 */

    while (kCount < n)
    {
        /* test next candidate k */
        uint64_t p1 = 30ULL * (uint64_t)nextK + 11;
        uint64_t p2 = 30ULL * (uint64_t)nextK + 13;

        /* ensure we have primes up to the larger of the two numbers */
        size_t required = (p1 > p2) ? (size_t)p1 : (size_t)p2;
        ensure_sieve(required);

        if (isPrime[p1] && isPrime[p2])
            store_k(nextK);

        ++nextK;
    }

    return (seq_t)kList[n - 1];
}
