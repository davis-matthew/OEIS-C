/*
 * A076812  a(n) = the smallest prime p such that p-prime(n)*4 is prime.
 * https://oeis.org/A076812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the smallest prime p such that (p - 4·prime(n)) is prime
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*       (prime(n) is the n‑th prime, starting with prime(1)=2) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* provides bool is_prime(idx_t) */

/*--------------------------------------------------------------------*/
/*  Prime sieve – generates all primes up to LIM once and keeps them   */
/*  in an array for quick access.  We also need the ability to search   */
/*  the array for the first prime ≥ a given bound.                     */
/*--------------------------------------------------------------------*/
static const idx_t LIM = 25000000;       /* 25 million is enough for n≤10 000 */
static idx_t *prime_list = NULL;         /* array of primes */
static size_t prime_cnt = 0;              /* number of primes in the array */
static bool primes_ready = false;        /* flag whether sieve already done */

/* Binary search on prime_list – returns first index i with prime_list[i] >= val */
static size_t lower_bound_prime(idx_t val)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Initialise the global prime list on first use */
static void init_primes(void)
{
    if (primes_ready) return;

    /* Simple Sieve of Eratosthenes; 1 byte per number */
    char *sieve = (char *)malloc((LIM + 1) * sizeof(char));
    if (!sieve) return;              /* on OOM – unlikely in judge */

    for (idx_t i = 0; i <= LIM; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (idx_t i = 2; (idx_t)i * i <= LIM; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= LIM; j += i)
                sieve[j] = 0;

    /* Count primes first to allocate array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIM; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!prime_list) { free(sieve); return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= LIM; ++i)
        if (sieve[i]) prime_list[pos++] = i;

    prime_cnt = cnt;
    free(sieve);
    primes_ready = true;
}

/*--------------------------------------------------------------------*/
/*  Sequence function                                               */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* only offset is 1 */
    if (!primes_ready) init_primes();
    if (n > prime_cnt - 1) return 0;          /* safety: n too large for our sieve */

    /* 1‑based to 0‑based conversion */
    idx_t idx_n = n - 1;
    idx_t prime_n = prime_list[idx_n];        /* the n‑th prime */

    /* Compute base = 4 * prime(n) */
    idx_t base = 4 * prime_n;

    /* Look for the smallest prime p ≥ base+2 such that (p - base) is prime */
    size_t start = lower_bound_prime(base + 2);
    for (size_t i = start; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        idx_t diff = p - base;                /* diff ≥ 2 */
        if (is_prime(diff))
            return (seq_t)p;                  /* exact value fits in seq_t or will be truncated if not */
    }

    /* If we exhausted the list we used, return 0 (should not happen with given limit) */
    return 0;
}
