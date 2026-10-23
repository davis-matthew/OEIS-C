/*
 * A154099  Maximal number of primes in intervals of size n containing the
 *          n-th prime.
 * https://oeis.org/A154099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max_{0≤k<n} #{p : p prime, p_n - n + k + 1 ≤ p ≤ p_n + k}
 *            where p_n is the n‑th prime
 * Range:     u8 n <= 2442, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool init_done = false;
static uint32_t *prime_prefix = NULL;     /* prefix prime counts */
static uint32_t *prime_list   = NULL;     /* list of primes */
static size_t   prime_cnt     = 0;         /* number of primes stored */
static size_t   prefix_len    = 0;         /* size of prime_prefix array */

/* use a reasonably large limit that covers the 10000th prime
   (104729) plus an extra margin.  Limit = 4 600 000 yields ~310 000 primes. */
static void init_primes(void)
{
    if (init_done) return;
    const size_t limit = 4600000;
    prefix_len = limit + 1;

    /* Allocate memory */
    uint8_t *is_prime = (uint8_t*)calloc(prefix_len, sizeof(uint8_t));
    if (!is_prime) exit(EXIT_FAILURE);
    prime_prefix = (uint32_t*)malloc(prefix_len * sizeof(uint32_t));
    if (!prime_prefix) exit(EXIT_FAILURE);

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Build prefix array and prime list */
    prime_prefix[0] = 0;
    for (size_t i = 1; i <= limit; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + is_prime[i];
        if (is_prime[i]) {
            prime_cnt++;
        }
    }

    /* Allocate prime list and fill it */
    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    /* Clean up */
    free(is_prime);

    init_done = true;
}

seq_t a(idx_t n)
{
    init_primes();

    /* Ensure n is within the available range */
    if ((idx_t)prime_cnt < n) {
        /* In practice this should never happen for the given limits. */
        return 0;
    }

    uint32_t p_n = prime_list[n - 1];           /* n-th prime (0‑based) */
    seq_t max_cnt = 0;

    for (idx_t k = 0; k < n; ++k) {
        /* left = p_n - n + k + 1 , right = p_n + k */
        idx_t left  = (idx_t)p_n - (idx_t)n + k + 1;
        if (left < 1) left = 1;
        idx_t right = (idx_t)p_n + k;

        /* Count primes in [left, right] using prefix sum */
        seq_t cnt = (seq_t)(prime_prefix[right] - prime_prefix[left - 1]);
        if (cnt > max_cnt) max_cnt = cnt;
    }
    return max_cnt;
}
