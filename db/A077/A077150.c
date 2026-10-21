/*
 * A077150  Number of composite numbers between n and 2n that are coprime to
 *          n.
 * https://oeis.org/A077150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) + π(n) – π(2 n)
 * Range:     u8 n <= 305, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a prime sieve up to 2·n on demand.
   It caches the sieve, the prefix prime counts and a list of primes
   for fast Euler ζ‑function calculations.  Subsequent calls use the
   cached data.  The algorithm runs in O(n log log n) time for the
   largest n requested, and is fast for thousands of random queries. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Static data used to cache the prime sieve and its helpers            */
/* ------------------------------------------------------------------ */
static idx_t   sieve_limit   = 0;          /* largest n for which the sieve is built */
static bool   *is_prime_arr  = NULL;      /* is_prime_arr[i] == true  iff i is prime */
static uint32_t *pi_prefix   = NULL;      /* pi_prefix[i] = π(i)           */
static idx_t   *prime_list   = NULL;      /* list of primes up to sieve_limit */
static size_t  prime_count   = 0;          /* number of primes stored       */

/* ------------------------------------------------------------------ */
/* Build or extend the sieve up to 'limit' (inclusive).                */
/* ------------------------------------------------------------------ */
static void
init_sieve(idx_t limit)
{
    if (limit <= sieve_limit)          /* already computed                         */
        return;

    /* free previous data if it exists                                */
    free(is_prime_arr);
    free(pi_prefix);
    free(prime_list);

    /* allocate arrays                                                        */
    is_prime_arr = (bool*)malloc((limit + 1) * sizeof(bool));
    pi_prefix    = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    prime_list   = (idx_t*)malloc((limit + 1) * sizeof(idx_t));

    if (!is_prime_arr || !pi_prefix || !prime_list) {
        /* Allocation failed – abort with zero result (should not happen).   */
        exit(1);
    }

    /* Initialise all as potentially prime                               */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime_arr[i] = true;
    if (limit >= 0) is_prime_arr[0] = false;
    if (limit >= 1) is_prime_arr[1] = false;

    /* Sieve of Eratosthenes                                               */
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (is_prime_arr[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_arr[j] = false;
        }
    }

    /* Build prefix prime counts and prime list                           */
    prime_count   = 0;
    pi_prefix[0]  = 0;
    if (limit >= 1)
        pi_prefix[1] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime_arr[i]) {
            prime_list[prime_count++] = i;
            pi_prefix[i] = pi_prefix[i - 1] + 1;
        } else {
            pi_prefix[i] = pi_prefix[i - 1];
        }
    }

    sieve_limit = limit;          /* remember largest limit built so far     */
}

/* ------------------------------------------------------------------ */
/* Compute Euler's totient φ(n) using the cached prime list.            */
/* ------------------------------------------------------------------ */
static seq_t
phi_calc(idx_t n)
{
    if (n == 0) return 0;          /* φ(0) is rarely used, return 0         */
    if (n == 1) return 1;          /* φ(1) = 1                               */

    seq_t phi = (seq_t)n;
    idx_t m   = n;

    for (size_t idx = 0; idx < prime_count &&
                         (idx_t)prime_list[idx] * (idx_t)prime_list[idx] <= m;
         ++idx)
    {
        idx_t p = prime_list[idx];
        if (m % p == 0) {
            /* p divides m – apply the formula φ *= (1 – 1/p)        */
            while (m % p == 0)
                m /= p;
            phi -= phi / p;
        }
    }
    if (m > 1) {          /* remaining prime factor larger than sqrt(n)  */
        phi -= phi / (seq_t)m;
    }
    return phi;
}

/* ------------------------------------------------------------------ */
/* Public function required by the test framework                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                 /* out of range – safe default             */
        return 0;

    /* Ensure the sieve covers up to 2·n for both φ and π calculations. */
    init_sieve(n + n);

    seq_t phi_val = phi_calc(n);                 /* φ(n)   */
    seq_t pi_n    = (seq_t)pi_prefix[n];         /* π(n)   */
    seq_t pi_2n   = (seq_t)pi_prefix[n + n];     /* π(2 n) */

    return phi_val + pi_n - pi_2n;
}
