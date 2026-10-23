/*
 * A128106  Sizes of possible gaps around a Gaussian prime: 1 and the even
 *          numbers in A001481.
 * https://oeis.org/A128106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the (n‑1)-th element of A001481, with a(1)=1. A001481 =
 *            the sorted even numbers that are representable as a^2+b^2
 * Range:     u8 n <= 16, u16 n <= 2162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

static seq_t *primes = NULL;
static size_t primesCount = 0;     /* number of primes currently stored */
static size_t primesCap   = 0;     /* allocated capacity of primes array   */
static seq_t  primesMaxFactor = 0; /* largest prime stored */

/* Ensure that primes[0..primesCount-1] contains all primes <= limit. */
static void ensure_primes_upto(seq_t limit)
{
    if (limit <= primesMaxFactor) return;

    if (!primes) {
        /* initial allocation: include 2 and 3 */
        primesCap = 16;
        primes = (seq_t *)malloc(primesCap * sizeof(seq_t));
        if (!primes) return;
        primes[0] = 2;
        primes[1] = 3;
        primesCount = 2;
        primesMaxFactor = 3;
    }

    /* start from the next odd candidate after the last stored prime */
    seq_t candidate = primes[primesCount - 1] + 2;
    for (; candidate <= limit; candidate += 2) {
        bool isPrime = true;
        for (size_t i = 0; i < primesCount; ++i) {
            seq_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { isPrime = false; break; }
        }
        if (isPrime) {
            if (primesCount == primesCap) {
                primesCap <<= 1;
                seq_t *t = (seq_t *)realloc(primes, primesCap * sizeof(seq_t));
                if (!t) return;
                primes = t;
            }
            primes[primesCount++] = candidate;
            primesMaxFactor = candidate;
        }
    }
}

/* Test whether even m can be expressed as a sum of two squares. */
static bool is_sum_two_squares(seq_t m)
{
    if ((m & (seq_t)1) != 0)   /* odd numbers (except 1) are impossible */
        return false;

    seq_t tmp   = m;
    seq_t limit = floor_sqrt(tmp);
    ensure_primes_upto(limit);

    for (size_t i = 0; i < primesCount; ++i) {
        seq_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if ((p % 4) == 3 && (exp & 1))  /* odd exponent of a 3 mod 4 prime */
                return false;
        }
    }
    if (tmp > 1 && ((tmp % 4) == 3))
        return false;
    return true;
}

/* Cache for sequence values */
static seq_t *cache        = NULL;
static size_t cacheSize    = 0;
static size_t cacheCap     = 0;
static seq_t  nextCandidate = 2;   /* next even integer to test */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* initialise cache on first call */
    if (!cache) {
        cacheCap   = 16;
        cache      = (seq_t *)calloc(cacheCap, sizeof(seq_t));
        if (!cache) return 0;
        cacheSize  = 1;          /* a(1) = 1 */
        cache[0]   = 1;
        nextCandidate = 2;
    }

    size_t target = (size_t)n;
    if (target > cacheCap) {
        size_t newCap = cacheCap ? cacheCap * 2 : 16;
        while (newCap < target) newCap <<= 1;
        seq_t *t = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!t) return 0;
        cache = t;
        cacheCap = newCap;
    }

    if (target > cacheSize) {
        for (size_t idx = cacheSize; idx < target; ++idx) {
            seq_t m;
            for (;;) {
                m = nextCandidate;
                if (is_sum_two_squares(m))
                    break;
                nextCandidate += 2;
            }
            cache[idx] = m;
            nextCandidate = m + 2;
        }
        cacheSize = target;
    }

    return cache[(size_t)n - 1];
}
