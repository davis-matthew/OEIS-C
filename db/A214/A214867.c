/*
 * A214867  Quotients of (first) primorial numbers and denominators of
 *          Bernoulli numbers B 0, B 1, B 2, B 4, B 6,... .
 * https://oeis.org/A214867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002110(n) / A006954(n) where A002110(n) is the product
 *            of the first n primes and A006954(n) is the denominator of
 *            Bernoulli number B_k with k = 0,1,2,4,6,... (k = 0 if n = 0, k =
 *            1 if n = 1, otherwise k = 2*n-2). The quotient can be obtained
 *            by multiplying all first n primes except those...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Prime cache utilities                                         */
static uint32_t *prime_cache = NULL;   /* array of primes */
static size_t   prime_cache_cnt = 0;   /* number of primes stored */

/* Estimate an upper bound for the n-th prime (Rosser) */
static idx_t estimate_prime_limit(idx_t n)
{
    if (n < 6) return 15;               /* p_6 = 13, a bit more safety */
    double dn = (double)n;
    double lim = dn * (log(dn) + log(log(dn))) + 3.0;
    return (idx_t)(lim) + 10;           /* extra margin          */
}

/* Sieve primes up to 'limit' and store them in prime_cache */
static void ensure_primes(idx_t n)
{
    if (prime_cache_cnt >= (size_t)n)
        return;                         /* already enough */

    idx_t limit = estimate_prime_limit(n);
    size_t sz = (size_t)(limit + 1);
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime)
        return;                         /* allocation failed */

    /* initialize */
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    /* classic Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i])
            ++cnt;

    /* allocate new cache */
    uint32_t *new_cache = (uint32_t *)calloc(cnt, sizeof(uint32_t));
    if (!new_cache)
    {
        free(is_prime);
        return;
    }

    /* store primes */
    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i])
            new_cache[idx++] = (uint32_t)i;

    /* replace old cache */
    free(prime_cache);
    prime_cache      = new_cache;
    prime_cache_cnt  = cnt;

    free(is_prime);
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Get at least the first n primes */
    ensure_primes(n);
    if (prime_cache_cnt < (size_t)n)
        return (seq_t)0;                /* safety; should not happen */

    /* Determine the index k of Bernoulli number B_k */
    idx_t k;
    if (n == 0)
        k = 0;
    else if (n == 1)
        k = 1;
    else
        k = 2 * n - 2;                  /* for n >= 2 */

    /* Compute the product of first n primes, skipping those
       whose (prime-1) divides k (and only when k > 0)             */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < (size_t)n; ++i)
    {
        uint32_t p = prime_cache[i];
        if (k > 0 && (k % (idx_t)(p - 1)) == 0)
            continue;                  /* skip: will cancel in denominator */
        prod *= (seq_t)p;
    }
    return prod;
}
