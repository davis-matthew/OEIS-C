/*
 * A145109  a(n) = 2*n * core(2*n).
 * https://oeis.org/A145109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n * core(2*n)
 * Range:     u8 n <= 10, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* core(m) = product of distinct prime divisors of m. */
#include <stdint.h>
#include <stdlib.h>

/* Global mutable cache for a small prime sieve. */
static uint64_t *primes      = NULL;  /* array of primes up to current limit   */
static size_t   primes_cnt  = 0;      /* number of primes                       */
static uint64_t primes_limit = 0;     /* maximum prime that was sieved         */

/* Create (or enlarge) the prime list up to the supplied limit.
   The function is called at most once per rising limit value. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= primes_limit)
        return;                           /* already sufficient                     */

    /* free the old sieve if any */
    free(primes);

    /* allocate temporary composite flag array */
    unsigned char *composite = (unsigned char *)malloc(limit + 1);
    if (!composite)
        exit(1);

    /* a zero value means “probably prime” */
    for (uint64_t i = 0; i <= limit; ++i)
        composite[i] = 0;

    /* classic sieve of Eratosthenes */
    for (uint64_t p = 2; p * p <= limit; ++p)
        if (!composite[p])
            for (uint64_t j = p * p; j <= limit; j += p)
                composite[j] = 1;

    /* count primes to allocate the array */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!composite[i])
            ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes)
        exit(1);

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!composite[i])
            primes[idx++] = i;

    primes_cnt   = cnt;
    primes_limit = limit;

    free(composite);
}

/* Return a(n) = 2*n * core(2*n) for n >= 1. */
seq_t a(idx_t n)
{
    /* 2*n fits into 64 bits for the test range (n <= 10 000). */
    uint64_t two_n = 2ULL * (uint64_t)n;

    /* Find a bound large enough to factor 2*n.  sqrt(2*n) is sufficient. */
    uint64_t lim = 0;
    while ((lim + 1) * (lim + 1) <= two_n)
        ++lim;
    ensure_primes(lim);

    /* Compute the square‑free kernel of 2*n. */
    uint64_t m = two_n;
    seq_t core = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > m)
            break;
        if (m % p == 0) {
            unsigned cnt = 0;
            do {
                m /= p;
                ++cnt;
            } while (m % p == 0);
            if (cnt & 1)          /* add p once for an odd exponent   */
                core *= (seq_t)p;
        }
    }
    if (m > 1)                  /* remaining prime factor            */
        core *= (seq_t)m;

    /* 2*n * core(2*n) */
    return (seq_t)two_n * core;
}
