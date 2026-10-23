/*
 * A023836  Sum of exponents in prime-power factorization of C(4n,2n-2).
 * https://oeis.org/A023836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents in prime‑power factorization of
 *            C(4n,2n‑2)
 * Range:     u8 n <= 599, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (directly as sum over primes of the difference of floor‑quotients) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* cache of primes up to the largest limit requested */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   primesCnt = 0;       /* number of primes stored */
static idx_t    maxPrimeLimit = 0;   /* largest value for which primes have been built */

/* build or extend a prime list up to at least 'limit' */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= maxPrimeLimit)
        return;          /* already have enough primes */

    free(primes);        /* discard old list if any */

    /* simple sieve of Eratosthenes */
    uint8_t *sieve = (uint8_t*)malloc((size_t)(limit + 1));
    if (!sieve)
        return;          /* allocation failure – unlikely */

    memset(sieve, 0, (size_t)(limit + 1));
    for (idx_t p = 2; p * p <= limit; ++p)
        if (!sieve[p])
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[q] = 1;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            ++cnt;

    /* allocate and fill prime list */
    uint64_t *newPrimes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!newPrimes) {
        free(sieve);
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit && idx < cnt; ++i)
        if (!sieve[i])
            newPrimes[idx++] = (uint64_t)i;

    free(sieve);
    primes = newPrimes;
    primesCnt = cnt;
    maxPrimeLimit = limit;
}

/* sequence function:  a(n) = Ω( C(4n, 2n−2) ) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* unused but harmless */

    const idx_t m  = 4 * n;          /* 4n */
    const idx_t k1 = 2 * n - 2;      /* 2n−2 */
    const idx_t k2 = 2 * n + 2;      /* 2n+2 */

    ensure_primes_upto(m);

    seq_t sum = 0;
    for (size_t pi = 0; pi < primesCnt; ++pi) {
        const idx_t p = (idx_t)primes[pi];
        if (p > m)
            break;
        idx_t pp = p;
        while (pp <= m) {
            seq_t term = (seq_t)(m / pp - k1 / pp - k2 / pp);
            sum += term;
            if (pp > m / p)
                break;
            pp *= p;
        }
    }
    return sum;
}
