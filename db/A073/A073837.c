/*
 * A073837  Sum of primes p satisfying n <= p <= 2n.
 * https://oeis.org/A073837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  A073837: Sum of primes p with n <= p <= 2n
 *
 *  Formula: a(n) = S(2n) - S(n‑1)
 *  where S(k) = sum of all primes <= k.
 *
 *  The implementation maintains a dynamic sieve of Eratosthenes and a
 *  prefix‑sum array of prime values.  For any requested n it extends
 *  the sieve only if necessary, then returns the difference of the
 *  two prefix sums.
 *
 *  All arithmetic uses seq_t for sums and idx_t for indices, avoiding
 *  overflow by performing additions directly in seq_t.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* required by the build:
 *  typedef idx_t;   // unsigned integer, e.g., uint64_t
 *  typedef seq_t;   // unsigned integer type of arbitrary width
 */

static idx_t current_limit = 0;
static uint8_t *is_prime = NULL;   /* 1 = prime, 0 = composite */
static seq_t *prime_sum = NULL;    /* prefix sums of primes */

/* extend sieve and prefix sums up to 'limit' (inclusive) */
static void extend_sieve(idx_t limit)
{
    if (limit <= current_limit) return;

    size_t new_size = (size_t)(limit + 1);
    uint8_t *tmp1 = (uint8_t *)realloc(is_prime, new_size * sizeof(uint8_t));
    seq_t *tmp2 = (seq_t  *)realloc(prime_sum, new_size * sizeof(seq_t));
    if (!tmp1 || !tmp2) {
        /* out of memory – abort safely */
        exit(1);
    }
    is_prime = tmp1;
    prime_sum = tmp2;

    /* initialise new entries as prime candidates */
    if (current_limit == 0) {
        /* first time: all entries set to 1, later we will mark composites */
        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = 0;
        is_prime[1] = 0;
    } else {
        for (idx_t i = current_limit + 1; i <= limit; ++i)
            is_prime[i] = 1;
    }

    /* sieve composites using primes already known up to sqrt(limit) */
    idx_t sqrt_lim = 0;
    while ((sqrt_lim + 1) * (sqrt_lim + 1) <= limit) sqrt_lim++;
    for (idx_t p = 2; p <= sqrt_lim; ++p) {
        if (!is_prime[p]) continue;
        /* the first multiple of p within the new range */
        idx_t start = (current_limit + 1 + p - 1) / p * p;
        if (start < p * p) start = p * p;
        for (idx_t j = start; j <= limit; j += p)
            is_prime[j] = 0;
    }

    /* recompute prefix sums for the new part */
    if (current_limit == 0) {
        prime_sum[0] = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            prime_sum[i] = prime_sum[i - 1] + (is_prime[i] ? (seq_t)i : 0);
        }
    } else {
        for (idx_t i = current_limit + 1; i <= limit; ++i) {
            prime_sum[i] = prime_sum[i - 1] + (is_prime[i] ? (seq_t)i : 0);
        }
    }

    current_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but harmless */

    /* ensure data up to 2n is available */
    extend_sieve(2 * n);

    seq_t total = prime_sum[2 * n];
    if (n > 1) total -= prime_sum[n - 1];
    return total;
}
