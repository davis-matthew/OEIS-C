/*
 * A120108  Number triangle T(n,k) = lcm(1,..,n+1)/lcm(1,..,k+1).
 * https://oeis.org/A120108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 77, u32 n <= 252, u64 n <= 1080,
 *            u128 n <= 3915, big any n
 * Helpers:   exponent_in_lcm, floor_sqrt_8n_plus_1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A120108: the flattened triangle T(n,k) = LCM(1..n+1)/LCM(1..k+1).
   For an overall index N (starting at 0) we find row r and column k such that
   N = r(r+1)/2 + k (0 ≤ k ≤ r) and then return the ratio of the two LCMs.
   Prime powers are handled by storing, for each prime p ≤ r+1,
     difference = ⌊log_p(r+1)⌋ – ⌊log_p(k+1)⌋
   and multiplying p^difference into the result. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/exponent_in_lcm.h"
#include "../utils/floor_sqrt_8n_plus_1.h"

/* Static storage for primes up to the largest needed limit. */
static idx_t *primes = NULL;     /* array of primes */
static size_t prime_count = 0;   /* number of primes stored   */
static idx_t max_prime_limit = 0;/* largest m for which primes are listed */

/* Ensure that the primes array contains all primes ≤ limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= max_prime_limit)
        return;

    /* Allocate and initialise a boolean array. */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime)
        return; /* allocation failure; caller will get zero result */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    /* Allocate new primes array. */
    idx_t *new_primes = NULL;
    if (count > 0) {
        new_primes = (idx_t *)malloc(count * sizeof(idx_t));
        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                new_primes[idx++] = i;
    }

    free(is_prime);

    /* Replace old list. */
    free(primes);
    primes = new_primes;
    prime_count = count;
    max_prime_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r and column k for the flattened index n. */
    idx_t root = floor_sqrt_8n_plus_1(n);
    idx_t r = (root - 1) / 2;

    /* triangular number of first r rows: r(r+1)/2 */
    __uint128_t triangular = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)triangular;

    /* Prepare primes up to r+1. */
    idx_t max_val = r + 1;
    idx_t col_val = k + 1;
    ensure_primes(max_val);

    /* Compute the ratio by multiplying the necessary prime powers. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p > max_val)
            break;
        idx_t exp_r = exponent_in_lcm(max_val, p);
        idx_t exp_k = exponent_in_lcm(col_val, p);
        idx_t diff   = exp_r > exp_k ? exp_r - exp_k : 0;
        for (idx_t j = 0; j < diff; ++j)
            result *= (seq_t)p;
    }

    return result;
}
