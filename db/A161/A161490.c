/*
 * A161490  a(n) = smallest prime of the form k*n+s(n) where s(n) is the sum
 *          of the first n primes, or a(n) = 0 if no such prime exists.
 * https://oeis.org/A161490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd, is_prime_big, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A161490: a(n) = smallest prime of the form k*n+s(n)
   where s(n) is the sum of the first n primes.  If n and s(n) are
   not coprime a(n)=0.  The function is memoized so repeated calls
   are fast. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility headers supplied by the build system */
#include "../utils/gcd.h"                 /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t n) */
#include "../utils/sieve_first_n_primes.h"/* void sieve_first_n_primes(idx_t n, uint32_t **p, size_t *cnt) */

/* Cached data */
static uint32_t *prime_list = NULL;   /* primes[0 .. prime_count-1] */
static size_t   prime_count = 0;      /* number of primes stored */
static seq_t   *prime_sums = NULL;    /* prefix sums, sums[0] = 0 */
static size_t   sum_count  = 0;       /* prime_count + 1 */

/* Rebuild the prime tables to contain at least 'needed' primes.  */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= (size_t)needed)
        return;                       /* already sufficient */

    /* Retrieve the first 'needed' primes via the provided sieve. */
    uint32_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    sieve_first_n_primes(needed, &new_primes, &new_cnt);

    /* Release old data if any. */
    if (prime_list)
        free(prime_list);
    if (prime_sums)
        free(prime_sums);

    prime_list  = new_primes;
    prime_count = new_cnt;

    /* Allocate and compute the prefix sums. */
    sum_count   = prime_count + 1;
    prime_sums  = (seq_t *)malloc(sizeof(seq_t) * sum_count);
    prime_sums[0] = (seq_t)0;
    for (size_t i = 1; i < sum_count; ++i) {
        /* Using cast to seq_t to avoid overflow of partial sums in
           small integer types (e.g., uint8_t). */
        prime_sums[i] = prime_sums[i-1] + (seq_t)prime_list[i-1];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset is 1, defensive for n=0 */
        return (seq_t)0;

    /* Make sure we have the first n primes and their sum. */
    ensure_primes(n);

    seq_t s = prime_sums[n];            /* sum of first n primes */
    seq_t g = gcd((seq_t)n, s);         /* gcd(n, s) */

    if (g != (seq_t)1)                  /* not coprime → no prime exists */
        return (seq_t)0;

    /* Search for the smallest prime k*n + s, k ≥ 1. */
    for (idx_t k = 1; ; ++k) {
        seq_t candidate = s + (seq_t)n * (seq_t)k;
        if (is_prime_big(candidate))
            return candidate;
    }

    /* Unreachable – keep compiler happy. */
    return (seq_t)0;
}
