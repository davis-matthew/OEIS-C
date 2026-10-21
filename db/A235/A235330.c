/*
 * A235330  Number of ways to write 2*n = p + q with p, q, prime(p) - p + 1
 *          and prime(q) + q + 1 all prime.
 * https://oeis.org/A235330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs of primes (p,q) * with p+q = 2*n,
 *            such that prime(p)-p+1 and prime(q)+q+1 * are also prime, where
 *            prime(k) denotes the k-th prime. * The sequence is indexed
 *            starting at n = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------- */
/*  Prime sieve and auxiliary data.                               */
/* --------------------------------------------------------------- */

#define SIEVE_LIMIT 300000U

static bool initialized = false;
static uint8_t *isPrimeArr = NULL;     /* 0/1 array of primality up to SIEVE_LIMIT */
static uint32_t *primeList = NULL;     /* primeList[1] = 2, primeList[2] = 3, ... */
static size_t primeCount = 0;

static void init_sieve(void)
{
    if (initialized)
        return;

    /* Allocate and initialise primality array. */
    isPrimeArr = (uint8_t *)malloc((size_t)(SIEVE_LIMIT + 1));
    if (!isPrimeArr) abort();
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        isPrimeArr[i] = 1;
    isPrimeArr[0] = isPrimeArr[1] = 0;

    /* Simple Eratosthenes sieve. */
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (isPrimeArr[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                isPrimeArr[j] = 0;
    }

    /* Count primes and allocate the prime list. */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (isPrimeArr[i])
            ++primeCount;

    primeList = (uint32_t *)malloc((primeCount + 1) * sizeof(uint32_t));
    if (!primeList) abort();

    size_t idx = 1;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (isPrimeArr[i])
            primeList[idx++] = i;

    initialized = true;
}

static inline bool isPrimeUint(uint32_t x)
{
    return x <= SIEVE_LIMIT ? isPrimeArr[x] : false;
}

/* --------------------------------------------------------------- */
/*  Sequence cache.                                               */
/* --------------------------------------------------------------- */

static seq_t *counts = NULL;
static idx_t counts_size = 0;

/* Count for a single n. */
static seq_t count_n(uint32_t n)
{
    seq_t cnt = 0;
    uint32_t limit = 2 * n;
    /* Iterate over all prime p <= limit */
    for (size_t i = 1; i <= primeCount; ++i) {
        uint32_t p = primeList[i];
        if (p > limit)
            break;

        uint32_t q = limit - p;
        if (q < 2)
            continue;
        if (!isPrimeUint(q))
            continue;

        /* Condition for p */
        if (p > primeCount)      /* should never happen for our limits */
            continue;
        uint32_t prime_of_p = primeList[p];
        uint32_t cond1 = prime_of_p - p + 1;
        if (cond1 > SIEVE_LIMIT || !isPrimeUint(cond1))
            continue;

        /* Condition for q */
        if (q > primeCount)      /* should never happen for our limits */
            continue;
        uint32_t prime_of_q = primeList[q];
        uint32_t cond2 = prime_of_q + q + 1;
        if (cond2 > SIEVE_LIMIT || !isPrimeUint(cond2))
            continue;

        ++cnt;
    }
    return cnt;
}

/* Ensure that counts[1..n] are computed. */
static void ensure_counts(idx_t n)
{
    if (n + 1 <= counts_size)
        return;

    seq_t *new_counts = (seq_t *)realloc(counts, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_counts) abort();
    counts = new_counts;

    /* Compute missing values. */
    used:
    for (idx_t i = counts_size; i <= n; ++i)
        counts[i] = count_n((uint32_t)i);

    counts_size = n + 1;
}

/* --------------------------------------------------------------- */
/*  Public API.                                                   */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* sequence offset starts at 1 */

    init_sieve();
    ensure_counts(n);
    return counts[n];
}
