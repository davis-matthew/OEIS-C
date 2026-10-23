/*
 * A045717  For each prime p take the sum of nonprimes < p.
 * https://oeis.org/A045717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^{p_n-1} k - sum_{prime q < p_n} q, where p_n is
 *            the n‑th prime. Equivalent to (p_n*(p_n-1))/2 minus the sum of
 *            all primes less than p_n
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global state for prime sieve and prefix sums. */
static size_t sieve_limit   = 0;
static bool *sieve_is_prime = NULL;
static uint64_t *prime_list = NULL;
static seq_t *prime_prefix  = NULL;
static size_t prime_count   = 0;

/* Build or rebuild the sieve up to 'limit' (inclusive).  All arrays
   are reallocated from scratch; this is acceptable for the limits
   required by the test harness. */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit)
        return;

    free(sieve_is_prime);
    free(prime_list);
    free(prime_prefix);

    sieve_is_prime = (bool *)malloc((limit + 1u) * sizeof(bool));
    if (!sieve_is_prime)
        return;                       /* Allocation failed – nothing more we can do. */

    for (size_t i = 0; i <= limit; ++i)
        sieve_is_prime[i] = true;
    if (limit >= 0) sieve_is_prime[0] = false;
    if (limit >= 1) sieve_is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve_is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_is_prime[j] = false;
        }
    }

    /* Count primes to allocate arrays. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve_is_prime[i]) ++cnt;
    prime_count = cnt;

    prime_list   = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    prime_prefix = (seq_t   *)malloc(cnt * sizeof(seq_t));
    if (!prime_list || !prime_prefix) {
        free(sieve_is_prime);
        sieve_is_prime = NULL;
        sieve_limit = 0;
        return;
    }

    seq_t running = 0;
    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve_is_prime[i]) {
            prime_list[k]   = (uint64_t)i;
            running += (seq_t)i;
            prime_prefix[k] = running;
            ++k;
        }
    }
    sieve_limit = limit;
}

/* Binary search: index of the first prime >= value.  If none, returns prime_count. */
static size_t first_prime_geq(uint64_t value)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;               /* The offset is 1; for n=0 we return 0. */

    /* Rough upper bound for the n‑th prime: p_n < n*(ln n + ln ln n).
       The factor 20 below is more than enough for n up to several millions. */
    size_t est_limit = n * 20u + 100u;
    if (est_limit < 15u) est_limit = 15u;
    ensure_sieve(est_limit);

    /* Make sure we have at least n primes; if not, extend the sieve. */
    while (prime_count < (size_t)n) {
        est_limit = est_limit * 2u + 100u;
        ensure_sieve(est_limit);
    }

    uint64_t p = prime_list[n - 1];          /* n-th prime */
    seq_t  p_seq = (seq_t)p;

    /* Sum of all integers less than p. */
    seq_t total_all = p_seq * (p_seq - 1u) / 2u;

    /* Sum of all primes less than p. */
    size_t idx = first_prime_geq(p);
    seq_t sum_primes = 0;
    if (idx > 0)
        sum_primes = prime_prefix[idx - 1];

    return total_all - sum_primes;
}
