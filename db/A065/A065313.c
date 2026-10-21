/*
 * A065313  a(n) = pi(n*pi(n)).
 * https://oeis.org/A065313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n * pi(n))
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation dynamically sieves primes up to the needed
   limit.  It caches the prime table and the prefix sum of primes
   (pi(x)).  All arithmetic on indices uses idx_t; prime counts are
   stored in seq_t.  The code is valid both as C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint8_t *is_prime = NULL;          /* 0 = composite, 1 = prime */
static seq_t *pi_table = NULL;            /* pi[i] = number of primes ≤ i   */
static idx_t sieve_limit = 0;             /* largest index currently sieved   */

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;    /* already enough */

    /* Resize storage for the new range */
    size_t new_size = (size_t)limit + 1U;
    uint8_t *tmp_prime = (uint8_t *)realloc(is_prime, new_size);
    seq_t *tmp_pi = (seq_t *)realloc(pi_table, new_size * sizeof(seq_t));
    if (!tmp_prime || !tmp_pi) return;    /* allocation failed – abort */

    is_prime = tmp_prime;
    pi_table = tmp_pi;

    /* Initialise the new entries as potential primes */
    for (idx_t i = sieve_limit + 1; i <= limit; ++i) is_prime[i] = 1;

    /* 0 and 1 are not prime */
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Sieve the extended range */
    idx_t r = isqrt_u64(limit);
    for (idx_t p = 2; p <= r; ++p) {
        if (is_prime[p]) {
            idx_t start = p * p;
            if (start < sieve_limit + 1)
                start = ((sieve_limit + 1 + p - 1) / p) * p;
            for (idx_t j = start; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Build the pi prefix (number of primes ≤ i) */
    seq_t cnt = (sieve_limit == 0 ? 0 : pi_table[sieve_limit]);
    for (idx_t i = sieve_limit + 1; i <= limit; ++i) {
        if (is_prime[i]) ++cnt;
        pi_table[i] = cnt;
    }
    if (sieve_limit == 0) {          /* initialise the very start */
        pi_table[0] = 0;
        if (limit >= 1) pi_table[1] = 0;
    }

    sieve_limit = limit;
}

/* a(n) = pi(n * pi(n)) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset is 1 */

    ensure_sieve(n);
    seq_t pi_n = pi_table[n];

    /* Compute n * pi(n) safely.  We assume pi_n fits in unsigned
       long long, which is safe for the required ranges. */
    idx_t prod = (idx_t)((__uint128_t)n * (unsigned long long)pi_n);

    ensure_sieve(prod);
    return pi_table[prod];
}
