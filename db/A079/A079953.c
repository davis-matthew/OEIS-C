/*
 * A079953  Smallest prime p such that prime(n) mod 2*p = prime(n).
 * https://oeis.org/A079953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p such that p > prime(n)/2
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* global storage for the prime list */
static idx_t *prime_list   = NULL;   /* one‑based index array of primes */
static size_t prime_count  = 0;      /* number of primes stored    */
static idx_t  prime_limit  = 0;      /* maximum value sieved       */

/* ------------------------------------------------------------------ */
/* Utility: ensure that at least 'needed' primes are available in      */
/*          prime_list.  The sieve is rebuilt when a larger limit is   */
/*          required.                                                 */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= needed)
        return;

    /* start with a small limit and double until enough primes */
    idx_t limit = prime_limit ? prime_limit * 2 : 1000;

    while (1) {
        /* allocation of the temporary boolean sieve */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime)
            exit(1);  /* allocation failure */

        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        /* Eratosthenes */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (idx_t m = p * p; m <= limit; m += p)
                    is_prime[m] = false;
            }
        }

        /* count primes up to the limit */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                ++cnt;

        if (cnt >= needed) {
            /* store the primes */
            idx_t *new_list = (idx_t *)malloc(cnt * sizeof(idx_t));
            if (!new_list)
                exit(1);
            size_t j = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (is_prime[i])
                    new_list[j++] = i;

            free(is_prime);
            free(prime_list);
            prime_list   = new_list;
            prime_count  = cnt;
            prime_limit  = limit;
            break;
        }

        /* not enough primes – expand the limit and try again */
        free(is_prime);
        limit <<= 1;  /* double the limit */
        /* guard against runaway limits (unlikely for n <= 10000) */
        if (limit > (idx_t)100000000)
            break;
    }
}

/* ------------------------------------------------------------------ */
/* Primality test using the current cache of primes.  Only odd       */
/* divisions are required for values up to prime_limit, which is       */
/* sufficient for the target range.                                   */
static bool is_prime_cached(idx_t x)
{
    if (x < 2)
        return false;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= x; ++i) {
        if (x % prime_list[i] == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n <= 0; return 0 as sentinel */

    /* ensure we have the first n primes */
    ensure_primes(n);

    idx_t p_n = prime_list[n - 1];        /* prime(n)          */
    idx_t half = p_n / 2;                 /* floor(pn/2)      */
    idx_t x = half + 1;                   /* candidate start  */

    /* search for the smallest prime > half */
    while (!is_prime_cached(x))
        ++x;

    return (seq_t)x;
}
