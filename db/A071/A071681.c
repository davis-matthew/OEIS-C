/*
 * A071681  Number of ways to represent the n-th prime as arithmetic mean of
 *          two other primes.
 * https://oeis.org/A071681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of unordered pairs of distinct primes (p,q) with
 *            p+q=2·p_n, where p_n is the nth prime. In other words, a(n)
 *            counts the Goldbach partitions of 2·p_n that use a prime smaller
 *            than p_n
 * Range:     u8 n <= 1857, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime64, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"
#include "../utils/is_prime64.h"

static idx_t *primes = NULL;      /* array of primes, 0-indexed */
static size_t prime_count = 0;    /* number of primes in the array   */
static idx_t sieve_limit = 0;     /* the limit used for the last sieve */

/* Ensure that we have at least `n` primes in the array. */
static void ensure_primes(idx_t n) {
    if (prime_count >= n) return;

    /* Estimate an upper bound for the nth prime (Rosser's theorem). */
    double nn = (double)n;
    double upper;
    if (n < 6) {
        upper = 15.0;
    } else {
        upper = nn * (log(nn) + log(log(nn))) + 10.0;
    }
    idx_t limit = (idx_t)ceil(upper);

    /* If the estimated limit is not larger than the current one,
       increase it to avoid cheap retries. */
    if (limit <= sieve_limit) {
        limit = sieve_limit * 2 + 1000;
    }

    /* Perform the sieve. */
    idx_t *newPrimes = NULL;
    size_t newCount;
    sieve_primes(limit, &newPrimes, &newCount);

    /* Replace the old prime array. */
    free(primes);
    primes = newPrimes;
    prime_count = newCount;
    sieve_limit = limit;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* outside the defined range */

    ensure_primes(n);               /* make sure the nth prime exists */

    /* nth prime (1‑indexed). */
    idx_t p_n = primes[n - 1];
    idx_t target = 2 * p_n;         /* we need primes summing to this value */

    seq_t count = 0;

    /* Count primes p < p_n such that target - p is prime. */
    for (idx_t i = 0; i + 1 < n; ++i) {
        idx_t p = primes[i];
        idx_t q = target - p;
        if (q != p && is_prime64(q)) {
            ++count;
        }
    }

    return count;
}
