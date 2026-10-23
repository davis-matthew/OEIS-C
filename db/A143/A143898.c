/*
 * A143898  Number of primes between n^K and (n+1)^K, where K =
 *          log(1151)/log(95).
 * https://oeis.org/A143898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = number of primes p such that n^K <= p <= (n+1)^K where K
 *            = log(1151)/log(95)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Our universal integral types */
extern seq_t a(idx_t n);

/* Static data for the prime sieve */
#define PRIME_LIMIT 3000000   /* covers (10001)^K ≈ 1.6e6 comfortably */

static bool *is_prime = NULL;
static uint32_t *prime_prefix = NULL;
static bool sieve_initialized = false;

/* Build sieve of Eratosthenes up to PRIME_LIMIT and compute prefix sums */
static void init_sieve(void) {
    size_t limit = PRIME_LIMIT;
    /* Allocate arrays */
    is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    prime_prefix = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!is_prime || !prime_prefix) {
        abort();
    }
    /* 0 and 1 are not primes */
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    /* Sieve */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t multiple = p * p; multiple <= limit; multiple += p)
                is_prime[multiple] = false;
        }
    }

    /* Prefix sum of primes */
    uint32_t count = 0;
    for (size_t i = 0; i <= limit; ++i) {
        if (is_prime[i]) ++count;
        prime_prefix[i] = count;
    }
}

/* Count primes in [l, r], inclusive. l,r are within [0, PRIME_LIMIT] */
static seq_t prime_count_range(uint64_t l, uint64_t r) {
    if (l == 0) {
        return prime_prefix[r];
    } else {
        return prime_prefix[r] - prime_prefix[l - 1];
    }
}

/* The main function as requested */
seq_t a(idx_t n) {
    /* Initialize sieve on first call */
    if (!sieve_initialized) {
        init_sieve();
        sieve_initialized = true;
    }

    /* Ensure that n is at least 1 as per OEIS offset */
    if (n < 1) {
        return 0;
    }

    /* Constant K = ln(1151)/ln(95) */
    const double K = log(1151.0) / log(95.0);

    /* Compute lower bound: ceil(n^K) */
    double low_d = pow((double)n, K);
    uint64_t low = (uint64_t)ceil(low_d - 1e-12);

    /* Compute upper bound: floor((n+1)^K) */
    double high_d = pow((double)(n + 1), K);
    uint64_t high = (uint64_t)floor(high_d + 1e-12);

    /* Clamp bounds to sieve limits */
    if (low > PRIME_LIMIT) low = PRIME_LIMIT;
    if (high > PRIME_LIMIT) high = PRIME_LIMIT;
    if (low > high) return 0;

    return prime_count_range(low, high);
}
