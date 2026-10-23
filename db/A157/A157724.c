/*
 * A157724  Numbers that are the sum of two distinct primes but are not twice
 *          a prime.
 * https://oeis.org/A157724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that are the sum of two distinct primes but not twice a
 *            prime
 * Range:     u8 n <= 148, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static bool *isPrime = NULL;
static uint64_t *primes = NULL;
static size_t primeCnt = 0;
static uint64_t *seq64 = NULL;
static size_t seqCnt = 0;

static void init(idx_t n)
{
    if (initialized)
        return;

    /* Choose a limit large enough to generate >10000 terms comfortably */
    const idx_t limit = 200000;

    /* Allocate and sieve primes up to 'limit' */
    isPrime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!isPrime)
        return; /* abort silently – the caller will get 0 */
    for (idx_t i = 0; i <= limit; ++i)
        isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;
    for (idx_t p = 2; (p * p) <= limit; ++p) {
        if (isPrime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                isPrime[m] = false;
        }
    }

    /* Store primes in an array */
    primeCnt = 0;
    primes = (uint64_t *)malloc((limit / 10 + 10) * sizeof(uint64_t));
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i])
            primes[primeCnt++] = i;

    /* Allocate space for sequence values */
    seq64 = (uint64_t *)malloc((limit + 10) * sizeof(uint64_t));
    seqCnt = 0;

    /* Generate the sequence up to 'limit' */
    for (idx_t num = 4; num <= limit; ++num) {
        /* Skip numbers that are twice a prime */
        if ((num % 2 == 0) && isPrime[num / 2])
            continue;

        bool ok = false;

        if (num % 2 == 1) {
            /* Odd numbers: only possible as 2 + an odd prime */
            if (num > 2 && isPrime[num - 2])
                ok = true;
        } else {
            /* Even numbers: look for two distinct odd primes */
            idx_t half = num / 2;
            for (size_t i = 0; i < primeCnt && primes[i] <= half; ++i) {
                uint64_t p = primes[i];
                uint64_t q = num - p;
                if (q == p) /* distinctness */
                    continue;
                if (isPrime[q]) {
                    ok = true;
                    break;
                }
            }
        }

        if (ok)
            seq64[seqCnt++] = num;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined but avoid crash */

    init(n); /* ensure sequence is ready */

    if (n <= seqCnt)
        return (seq_t)seq64[n - 1];
    else
        return 0; /* out of precomputed range – should not happen for n ≤ 10000 */
}
