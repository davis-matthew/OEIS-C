/*
 * A138225  Number of pairs (p,q) of primes with p < q < prime(n), p + q >
 *          prime(n), q + prime(n) > p and prime(n) + p > q.
 * https://oeis.org/A138225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime pairs (p,q) with p < q < prime(n), p + q
 *            > prime(n) (the other two triangle inequalities are automatic)
 * Range:     u8 n <= 38, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/sieve_primes.h"

/* Cached primes up to the current sieve limit. */
static idx_t *prime_cache   = NULL;  /* array of primes <= current limit */
static size_t prime_cnt     = 0;      /* number of primes in the array   */
static idx_t  prime_limit   = 0;      /* current sieve limit             */

/* Rebuild the sieve cache up to 'limit' (inclusive). */
static void rebuild_cache(idx_t limit)
{
    if (prime_cache) {
        free(prime_cache);
        prime_cache   = NULL;
        prime_cnt     = 0;
        prime_limit   = 0;
    }

    if (limit < 2) {
        return;                     /* no primes to cache */
    }

    /* sieve_primes allocates memory for the array internally. */
    sieve_primes(limit, &prime_cache, &prime_cnt);
    prime_limit = limit;
}

/* Count pairs (p_i , p_j) among primes < p_n that satisfy p_i + p_j > p_n. */
static seq_t count_pairs(idx_t p_n)
{
    /* Find the number of primes strictly less than p_n. */
    size_t m = 0;
    while (m < prime_cnt && prime_cache[m] < p_n) {
        ++m;
    }
    if (m < 2) {
        return (seq_t)0;
    }

    seq_t res = (seq_t)0;
    size_t i = 0;
    size_t j = m - 1;

    while (i < j) {
        idx_t sum = prime_cache[i] + prime_cache[j];
        if (sum > p_n) {
            /* All pairs (i, k) for k = i+1 .. j satisfy the condition. */
            res += (seq_t)(j - i);
            j--;          /* move the upper index left for next iteration */
        } else {
            i++;          /* sum too small, need a larger lower index */
        }
    }
    return res;
}

/* Main entry function required by the testcase harness */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    idx_t p_n = get_prime(n);          /* n-th prime */

    /* Rebuild cache if we haven't sieved far enough yet */
    if (p_n > prime_limit) {
        rebuild_cache(p_n);
    }

    return count_pairs(p_n);
}
