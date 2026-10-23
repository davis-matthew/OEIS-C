/*
 * A160058  Primes whose distance to both nearest neighbor primes is not of
 *          the form 2^k.
 * https://oeis.org/A160058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose distance to both nearest neighbor primes is NOT a power of two */

/* Uses build_primes_upto from the utils library.
   The function generates all primes <= limit and stores them in a caller‑allocated
   array of uint64_t.  We keep the primes in memory and scan them to precompute
   the qualifying primes.  The cache is lazily expanded until the requested
   n‑th element can be returned. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* declares: void build_primes_upto(idx_t, uint64_t **, size_t *); */

static uint64_t *primeList = NULL;   /* array of all primes up to sieveLimit */
static size_t     primeCnt  = 0;      /* count of primes in primeList */
static idx_t      sieveLimit = 0;     /* maximum number that has been sieved */

static seq_t *qualPrimes = NULL;     /* primes satisfying the distance condition */
static size_t qualCount  = 0;        /* number of qualifying primes stored */

static inline bool is_pow2(uint64_t v)
{
    return v && (v & (v - 1)) == 0;
}

/* rebuild the prime sieve and the qualifying prime cache up to at least 'limit' */
static void rebuild_cache(idx_t limit)
{
    /* free previous allocations */
    if (primeList) free(primeList);
    if (qualPrimes) free(qualPrimes);

    /* allocate primes up to 'limit' */
    build_primes_upto(limit, (uint64_t **)&primeList, &primeCnt);

    /* allocate space for qualifying primes (worst case: all primes qualify) */
    qualPrimes = (seq_t *)malloc(primeCnt * sizeof(seq_t));
    qualCount  = 0;

    /* scan between the second and second‑last primes; the first and last
       primes cannot have gaps on both sides yet */
    for (size_t i = 1; i + 1 < primeCnt; ++i)
    {
        uint64_t prev = primeList[i - 1];
        uint64_t cur  = primeList[i];
        uint64_t next = primeList[i + 1];
        uint64_t gap1 = cur - prev;
        uint64_t gap2 = next - cur;

        if (!is_pow2(gap1) && !is_pow2(gap2))
            qualPrimes[qualCount++] = (seq_t)cur;
    }

    sieveLimit = limit;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index 0 is undefined; OEIS offset is 1 */

    /* initial build if not yet done */
    if (sieveLimit == 0)
    {
        /* start with a modest limit; the cache will enlarge as needed */
        rebuild_cache(20000ULL);
        if (qualCount >= (size_t)n) return qualPrimes[n - 1];
    }

    /* grow the sieve until we have at least n qualifying primes */
    while (qualCount < (size_t)n)
    {
        idx_t newLimit = sieveLimit * 2;
        if (newLimit <= sieveLimit) newLimit = sieveLimit + 100000ULL; /* safety */
        rebuild_cache(newLimit);
    }

    /* now we can return the requested element */
    return qualPrimes[n - 1];
}
