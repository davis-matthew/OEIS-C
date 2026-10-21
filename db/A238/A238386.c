/*
 * A238386  a(n) = |{0 < k < n-1: p = prime(k) + pi(n-k) and p + 2 are both
 *          prime}|, where pi(.) is given by A000720.
 * https://oeis.org/A238386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n-1 : p = prime(k) + pi(n-k) and both p and p+2
 *            are prime}| This function uses a static sieve to compute primes
 *            up to a limit that grows with the requested n. It returns the
 *            count as type seq_t
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/*---------------------------------------------------------------
   Local cache for primes and the prime counting function pi
  ---------------------------------------------------------------*/
static unsigned char *prime_sieve = NULL;  /* 0/1 array, 1=prime */
static size_t sieve_limit = 0;             /* highest index of the sieve */

static uint32_t *primes = NULL;            /* list of primes */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static idx_t *pi_arr = NULL;               /* pi[i] = number of primes <= i */
static size_t pi_limit = 0;                /* number of entries in pi_arr */

/*---------------------------------------------------------------
   Ensure that the sieve contains all primes up to at least n
  ---------------------------------------------------------------*/
static void ensure_limit(idx_t n)
{
    /* heuristic: n-th prime < 20*n for n <= 1e7 */
    size_t limit = (size_t)(n * 20) + 100;
    if (limit <= sieve_limit)
        return;                /* already big enough */

    /* free old data */
    free(prime_sieve);
    free(pi_arr);
    free(primes);

    /* allocate sieve array */
    sieve_limit = limit;
    prime_sieve = (unsigned char *)calloc(sieve_limit + 1, sizeof(unsigned char));
    if (!prime_sieve)
        return;   /* out of memory – in practice this should never happen */

    /* initial assumption: all numbers are prime */
    for (size_t i = 2; i <= limit; ++i)
        prime_sieve[i] = 1;
    prime_sieve[0] = prime_sieve[1] = 0;

    /* Eratosthenes sieve */
    for (size_t i = 2; i * i <= limit; ++i)
        if (prime_sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                prime_sieve[j] = 0;

    /* build list of primes */
    primes_cap = limit / 10 + 1;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (prime_sieve[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    /* build pi array */
    pi_limit = limit + 1;
    pi_arr = (idx_t *)malloc(pi_limit * sizeof(idx_t));
    idx_t count = 0;
    pi_arr[0] = 0;
    for (size_t i = 1; i <= limit; ++i) {
        if (prime_sieve[i]) ++count;
        pi_arr[i] = count;
    }
}

/*---------------------------------------------------------------
   Main sequence function
  ---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n <= 2) return 0;                  /* a(1)=0, a(2)=0 */

    ensure_limit(n);                       /* build sieve up to required size */

    seq_t cnt = 0;

    for (idx_t k = 1; k + 1 < n; ++k) {    /* 0 < k < n-1 */
        uint32_t pk = primes[k - 1];       /* prime(k) */
        idx_t p = pk + pi_arr[n - k];      /* p = prime(k) + pi(n-k) */
        idx_t p2 = p + 2;

        bool p_ok  = (p <= sieve_limit) ? prime_sieve[p] : is_prime(p);
        bool p2_ok = (p2 <= sieve_limit) ? prime_sieve[p2] : is_prime(p2);

        if (p_ok && p2_ok)
            ++cnt;
    }

    return cnt;
}
