/*
 * A038107  Number of primes < n^2.
 * https://oeis.org/A038107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes < n^2 = prime_pi(n^2 - 1) (for n ≥ 2,
 *            n^2 is never prime so the inequality is strict)
 * Range:     u8 n <= 40, u16 n <= 906, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper functions from the utilities */
#include "../utils/is_prime.h"  /* provides isPrimeGeneric etc, not used directly */

/* Use the provided floor sqrt routine */
#include "../utils/_floor_isqrt_u64.h"

static uint8_t *isPrimeMask = NULL;      /* bitset of composites (0 = prime) */
static size_t maskSize = 0;              /* number of bytes allocated in isPrimeMask */

static uint32_t *primeList = NULL;       /* array of primes discovered so far */
static size_t primeCap = 0;              
static size_t primeCnt = 0;             

static idx_t currentMax = 0;             /* largest n^2 processed so far */

/* Set a number as composite in the bitset */
static inline void setComposite(idx_t n)
{
    isPrimeMask[n >> 3] |= (uint8_t)(1U << (n & 7));
}

/* Test if a number is composite in the bitset */
static inline bool isComposite(idx_t n)
{
    return (isPrimeMask[n >> 3] & (1U << (n & 7))) != 0;
}

/* Extend the sieve to encompass all numbers up to 'limit'. */
static void extendSieve(idx_t limit)
{
    /* Grow the bitset if necessary */
    size_t newMaskSize = (limit >> 3) + 1;
    if (isPrimeMask == NULL) {
        isPrimeMask = (uint8_t *)malloc(newMaskSize);
        if (!isPrimeMask) exit(1);
        for (size_t i = 0; i < newMaskSize; ++i) isPrimeMask[i] = 0;
    } else if (newMaskSize > maskSize) {
        isPrimeMask = (uint8_t *)realloc(isPrimeMask, newMaskSize);
        if (!isPrimeMask) exit(1);
        for (size_t i = maskSize; i < newMaskSize; ++i) isPrimeMask[i] = 0;
    }
    maskSize = newMaskSize;

    /* Mark 0 and 1 as composite */
    if (limit >= 0) setComposite(0);
    if (limit >= 1) setComposite(1);

    /* Mark composites in the new range using existing primes */
    idx_t start = (currentMax > 1) ? currentMax + 1 : 2;
    idx_t sieveLimit = _floor_isqrt_u64(limit);
    for (size_t i = 0; i < primeCnt; ++i) {
        idx_t p = primeList[i];
        if (p > sieveLimit) break;
        idx_t mstart = p * p;
        if (mstart < start) mstart = ((start + p - 1) / p) * p;
        for (idx_t j = mstart; j <= limit; j += p) setComposite(j);
    }

    /* Find new primes in the added region */
    for (idx_t n = start; n <= limit; ++n) {
        if (!isComposite(n)) {
            if (primeCnt == primeCap) {
                primeCap = (primeCap == 0) ? 256 : primeCap * 2;
                primeList = (uint32_t *)realloc(primeList, primeCap * sizeof(uint32_t));
                if (!primeList) exit(1);
            }
            primeList[primeCnt++] = (uint32_t)n;
        }
    }

    currentMax = limit;
}

/* Binary search to count primes ≤ limit */
static seq_t countPrimesUpTo(idx_t limit)
{
    size_t lo = 0, hi = primeCnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)primeList[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* The required OEIS sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;            /* a(0)=a(1)=0 */
    idx_t sq = n * n;                /* n^2 */
    idx_t limit = sq - 1;            /* primes < n^2 */

    if (limit > currentMax) extendSieve(limit);

    return countPrimesUpTo(limit);
}
