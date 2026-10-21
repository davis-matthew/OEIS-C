/*
 * A237497  a(n) = |{0 < k <= n/2: pi(k*(n-k)) is prime}|, where pi(.) is
 *          given by A000720.
 * https://oeis.org/A237497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k <= n/2: pi(k*(n-k)) is prime}|
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Manage a simple sieve of Eratosthenes up to a limit that grows
   as necessary when a(k*(n-k)) requires primes in a larger range.  */
static uint8_t *is_prime = NULL;       /* is_prime[i] == 1 if i is prime */
static uint32_t *primes   = NULL;      /* list of primes in order */
static size_t   primes_cnt = 0;        /* number of entries in primes[] */
static idx_t    curr_limit = 0;        /* maximum m that has been sieved */

/* Reallocate the sieve so that it contains all numbers up to limit.
   limit is chosen as n*n/4, the maximum possible value of k*(n-k). */
static void recompute_sieve(idx_t n)
{
    idx_t limit = (n * n) / 4;
    if (limit <= curr_limit) return;          /* already sufficient */

    free(is_prime);
    free(primes);
    is_prime = NULL;
    primes   = NULL;
    if (limit == 0) limit = 1;                 /* avoid zero-byte malloc */

    is_prime = (uint8_t *)malloc((size_t)limit + 1);
    if (!is_prime) return;

    /* initialise all entries to 1 */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* classic sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[q] = 0;
        }
    }

    /* count how many primes we have found */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) return;
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;

    curr_limit = limit;
}

/* Count how many array values are <= key (upper bound).  */
static size_t upper_bound_uint32(const uint32_t *arr, size_t n,
                                 uint32_t key)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] <= key) lo = mid + 1;
        else                 hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    recompute_sieve(n);

    idx_t half = n / 2;
    seq_t count = 0;

    for (idx_t k = 1; k <= half; ++k) {
        idx_t m = k * (n - k);              /* m = k*(n-k) */
        /* pi(m) = number of primes <= m:
           it is the index of the first prime > m, i.e. upper_bound. */
        size_t pi_m = upper_bound_uint32(primes, primes_cnt,
                                         (uint32_t)m);
        /* check if pi(m) is itself prime; we have precomputed primality
           up to curr_limit (>= pi_m always for m <= limit). */
        if (pi_m <= curr_limit && is_prime[pi_m])
            ++count;
    }
    return count;
}
