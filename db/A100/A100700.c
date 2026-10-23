/*
 * A100700  n-th Fibonacci number minus n-th prime number.
 * https://oeis.org/A100700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = F_n - p_n where F_1=1, F_2=1 (standard Fibonacci) and
 *            p_1=2 is the first prime
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/estimate_prime_upper_bound.h"

/* Cached list of primes */
static uint64_t *prime_list = NULL;
static idx_t prime_count = 0;
static idx_t sieve_limit   = 0;

/* Simple sieve of Eratosthenes storing only primes up to 'limit' */
static void sieve_primes(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already computed enough */

    /* Allocate array marking composites */
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, 1);
    if (!is_composite) return;                 /* allocation failure, abort */

    idx_t i, j;
    idx_t sq = (idx_t)(sqrt((double)limit));
    for (i = 2; i <= sq; ++i) {
        if (!is_composite[i]) {
            for (j = i * i; j <= limit; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* Count primes for allocation */
    idx_t need = 0;
    for (i = 2; i <= limit; ++i) if (!is_composite[i]) ++need;

    /* Reallocate prime list if needed */
    uint64_t *new_list = (uint64_t *)realloc(prime_list, need * sizeof(uint64_t));
    if (!new_list) { free(is_composite); return; }
    prime_list = new_list;

    /* Fill prime list */
    idx_t idx = 0;
    for (i = 2; i <= limit; ++i) if (!is_composite[i]) {
        prime_list[idx++] = (uint64_t)i;
    }

    free(is_composite);
    prime_count  = need;
    sieve_limit  = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* out of bounds, not used */

    /* Ensure primes up to n are available */
    idx_t bound = estimate_prime_upper_bound(n);
    sieve_primes(bound);
    if (prime_count < n) {
        /* fallback: increase bound and retry (unlikely for n <= 10000) */
        bound = estimate_prime_upper_bound(n + 10);
        sieve_primes(bound);
    }

    /* Compute F_n by linear addition */
    seq_t prev = 0;          /* F_0 */
    seq_t cur  = 1;          /* F_1 */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t tmp  = cur;
        cur = cur + prev;
        prev = tmp;
    }
    /* For n==1, cur is already F_1 */

    /* Retrieve nth prime (1-indexed in prime_list) */
    seq_t prime = (seq_t)prime_list[n - 1];

    /* Return difference */
    return cur - prime;
}
