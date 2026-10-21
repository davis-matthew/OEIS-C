/*
 * A085970  Number of integers ranging from 2 to n that are not prime-powers.
 * https://oeis.org/A085970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 1 - #{prime‑powers ≤ n}
 * Range:     u8 n <= 341, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of integers in [2, n] that are not prime powers. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static cache for a sieve */
static size_t sieve_limit = 0;
static unsigned char *isPrime = NULL;       /* 0 = composite, 1 = prime */
static uint32_t  *primePrefix = NULL;      /* prefix count of primes */

/* Build a simple sieve up to 'limit' (inclusive) and fill primePrefix. */
static void build_sieve(size_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate space for the sieve and prefix array */
    isPrime      = (unsigned char *)realloc(isPrime,      limit + 1);
    primePrefix  = (uint32_t   *)realloc(primePrefix,  (limit + 1) * sizeof(uint32_t));
    if (!isPrime || !primePrefix)
        exit(1);   /* out of memory */

    /* Initially mark all numbers as prime (1). */
    for (size_t i = 0; i <= limit; ++i)
        isPrime[i] = 1;
    if (limit >= 0) isPrime[0] = 0;
    if (limit >= 1) isPrime[1] = 0;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                isPrime[j] = 0;
        }
    }

    /* Build prefix counts of primes */
    primePrefix[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        primePrefix[i] = primePrefix[i - 1] + (isPrime[i] ? 1 : 0);

    sieve_limit = limit;
}

/* a(n)  ->  number of integers in [2,n] that are not prime powers   */
seq_t a(idx_t n)
{
    if (n <= 1)        /* a(1) = 0 by definition */
        return 0;

    /* Prepare sieve up to n */
    build_sieve((size_t)n);

    /* Count primes ≤ n */
    uint32_t prime_cnt = primePrefix[n];

    /* Count prime powers with exponent ≥ 2 */
    unsigned long long exp_cnt = 0;
    size_t sqrt_n = (size_t)sqrtl((long double)n);

    for (size_t p = 2; p <= sqrt_n; ++p) {
        if (isPrime[p]) {
            unsigned __int128 power = (unsigned __int128)p * p;
            while (power <= n) {
                ++exp_cnt;
                if (power > n / p)   /* next multiplication would overflow n */
                    break;
                power *= p;
            }
        }
    }

    /* Total prime‑powers ≤ n */
    unsigned long long prime_power_cnt = (unsigned long long)prime_cnt + exp_cnt;

    /* Non-prime‑powers in [2,n] */
    unsigned __int128 nn   = n;  /* use 128‑bit for intermediate safety */
    unsigned __int128 res  = nn - 1 - (unsigned __int128)prime_power_cnt;

    return (seq_t)res;
}
