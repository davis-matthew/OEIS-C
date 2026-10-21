/*
 * A240697  The smallest p2-p1 when 2n = p1+p4 = p2+p3, where p1 < p2 <= p3 <
 *          p4 are prime numbers.
 * https://oeis.org/A240697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest difference between the smaller primes in
 *            distinct Goldbach partitions of 2n, or 0 if fewer than two
 *            partitions exist
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Global prime sieve cache */
static char *primeTable = NULL;
static idx_t currentPrimeLimit = 0;

/* Global flag array to mark the smaller primes in each representation */
static uint8_t *smallPrimeFlag = NULL;
static idx_t currentUsedLimit = 0;

/* Build an Eratosthenes sieve up to 'limit' (inclusive). */
static void rebuild_prime_table(idx_t limit)
{
    free(primeTable);
    primeTable = (char*)malloc((size_t)limit + 1);
    if (!primeTable) { currentPrimeLimit = 0; return; }

    for (idx_t i = 0; i <= limit; ++i) primeTable[i] = 1;
    primeTable[0] = primeTable[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (primeTable[i])
            for (idx_t j = i * i; j <= limit; j += i)
                primeTable[j] = 0;

    currentPrimeLimit = limit;
}

/* Ensure the smallPrimeFlag array is large enough for 'limit'. */
static void ensure_small_prime_flag(idx_t limit)
{
    if (limit <= currentUsedLimit) return;
    free(smallPrimeFlag);
    smallPrimeFlag = (uint8_t*)malloc((size_t)limit + 1);
    if (!smallPrimeFlag) { currentUsedLimit = 0; return; }
    currentUsedLimit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t need = 2 * n;                     /* S = 2n */
    if (need < 4) return (seq_t)0;           /* not enough to have two primes */

    /* Prepare prime table if necessary. */
    if (!primeTable || need > currentPrimeLimit)
        rebuild_prime_table(need);
    if (!primeTable) return (seq_t)0;         /* allocation failure */

    /* Prepare flag array for smaller primes. */
    ensure_small_prime_flag(need);
    if (!smallPrimeFlag) return (seq_t)0;     /* allocation failure */
    memset(smallPrimeFlag, 0, (size_t)need + 1);

    /* Find all Goldbach pairs and mark the smaller prime. */
    size_t smallCount = 0;
    for (idx_t p = 2; p <= need / 2; ++p) {
        if (!primeTable[p]) continue;
        idx_t q = need - p;
        if (!primeTable[q]) continue;
        smallPrimeFlag[p] = 1;
        ++smallCount;
    }

    if (smallCount < 2) return (seq_t)0;      /* fewer than two representations */

    /* Compute the minimal difference between consecutive marked primes. */
    seq_t minDiff = 0;
    seq_t prevPrime = 0;
    for (idx_t x = 2; x <= need; ++x) {
        if (!smallPrimeFlag[x]) continue;
        if (prevPrime) {
            seq_t diff = (seq_t)x - prevPrime;
            if (minDiff == 0 || diff < minDiff) minDiff = diff;
            if (minDiff == 1) break;           /* cannot be smaller */
        }
        prevPrime = (seq_t)x;
    }

    return minDiff;
}
