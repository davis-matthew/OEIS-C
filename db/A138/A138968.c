/*
 * A138968  Positions of the primes congruent to 1 mod 3 when all primes
 *          except 3 are listed in order.
 * https://oeis.org/A138968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   positions of primes congruent to 1 (mod 3) in the prime list
 *            when all primes except 3 are listed in order
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Test whether x is prime using the primes stored in primes[0 .. size-1] */
static bool is_prime_candidate(uint64_t x, uint64_t *primes, size_t size)
{
    if (x < 2) return false;
    for (size_t i = 0; i < size; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;           /* avoid overflow: p*p > x */
        if (x % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, just return 0 */

    /* static mutable state */
    static uint64_t *primes = NULL;
    static size_t primesCap = 0;
    static size_t primesSize = 0;
    static uint64_t lastChecked = 0;     /* last number tested for primality */

    static seq_t *cache = NULL;          /* cached positions of primes ≡1 (mod 3) */
    static size_t cacheCap = 0;
    static size_t cacheSize = 0;
    static seq_t curPos = 0;             /* number of primes (≠3) seen so far */

    /* ensure cache has at least n entries */
    if (cacheSize < (size_t)n) {
        /* allocate or grow cache */
        if (cache == NULL) {
            cacheCap = 128;
            cache = (seq_t *)malloc(cacheCap * sizeof(seq_t));
        } else if (cacheSize >= cacheCap) {
            cacheCap *= 2;
            cache = (seq_t *)realloc(cache, cacheCap * sizeof(seq_t));
        }

        /* initialise primes array if empty */
        if (primesSize == 0) {
            if (primesCap == 0) {
                primesCap = 128;
                primes = (uint64_t *)malloc(primesCap * sizeof(uint64_t));
            }
            primes[0] = 2;          /* first prime */
            primesSize = 1;
            lastChecked = 2;
            /* process prime 2 */
            curPos++;                /* 2 ≠ 3 */
            /* 2 % 3 == 2 ⇒ not cached */
        }

        /* generate primes until the cache contains at least n positions */
        while (cacheSize < (size_t)n) {
            uint64_t candidate;
            if (lastChecked == 2)
                candidate = 3;        /* next prime after 2 */
            else
                candidate = lastChecked + 2;  /* skip even numbers */

            /* find next prime */
            while (!is_prime_candidate(candidate, primes, primesSize))
                candidate += 2;

            /* store the new prime */
            if (primesSize >= primesCap) {
                primesCap *= 2;
                primes = (uint64_t *)realloc(primes, primesCap * sizeof(uint64_t));
            }
            primes[primesSize++] = candidate;
            lastChecked = candidate;

            /* handle the prime */
            if (candidate != 3) {
                curPos++;              /* count it in the positions list */
                if (candidate % 3 == 1)
                    cache[cacheSize++] = curPos;  /* record its position */
            }
        }
    }

    return cache[n - 1];
}
