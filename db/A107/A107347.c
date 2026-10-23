/*
 * A107347  Number of even semiprimes strictly between prime(n) and
 *          2*prime(n).
 * https://oeis.org/A107347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 - pi(prime(n)/2) (OEIS A107347)
 * Range:     u8 n <= 54, u16 n <= 7617, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* utility from the build environment */
#include "../utils/get_prime.h"

/* ---------- static cache for prime sieve and prefix array ---------- */
static uint8_t *is_prime_arr = NULL;   /* 1 if index is prime, else 0 */
static idx_t  *pi_prefix_arr = NULL;   /* pi_prefix_arr[i] = #{primes ≤ i} */
static idx_t  sieve_max = 0;           /* largest index that has been sieved */

/* rebuild sieve and pi_prefix_arr up to max_limit (inclusive) */
static void
ensure_sieve(idx_t max_limit)
{
    if (max_limit <= sieve_max)
        return;                /* already sufficient */

    /* free old tables if any */
    if (is_prime_arr) {
        free(is_prime_arr);
        is_prime_arr = NULL;
    }
    if (pi_prefix_arr) {
        free(pi_prefix_arr);
        pi_prefix_arr = NULL;
    }

    /* allocate arrays (cast for C++ compatibility) */
    size_t sz = (size_t)(max_limit + 1);
    is_prime_arr = (uint8_t *)calloc(sz, sizeof(uint8_t));
    pi_prefix_arr = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!is_prime_arr || !pi_prefix_arr) {
        /* allocation failed; next calls will try again */
        return;
    }

    /* simple sieve of Eratosthenes */
    if (max_limit >= 0)
        is_prime_arr[0] = 0;
    if (max_limit >= 1)
        is_prime_arr[1] = 0;
    for (idx_t i = 2; i <= max_limit; ++i)
        is_prime_arr[i] = 1;
    for (idx_t i = 2; (i * i) <= max_limit; ++i) {
        if (is_prime_arr[i]) {
            for (idx_t j = i * i; j <= max_limit; j += i)
                is_prime_arr[j] = 0;
        }
    }

    /* build prefix array of pi */
    idx_t count = 0;
    for (idx_t i = 0; i <= max_limit; ++i) {
        if (i >= 2 && is_prime_arr[i])
            ++count;
        pi_prefix_arr[i] = count;
    }

    sieve_max = max_limit;
}

/* ---------- main sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Get the nth prime: p_n = prime(n) */
    seq_t pn_seq = get_prime((uint64_t)n);   /* get_prime returns seq_t */
    uint64_t pn_val = (uint64_t)pn_seq;      /* assume fits for required n */

    /* Compute upper bounds needed for sieve */
    idx_t limit_needed = (idx_t)(2 * pn_val);
    if (limit_needed < pn_val)
        limit_needed = pn_val;                /* prevent overflow for huge primes */

    ensure_sieve(limit_needed);

    /* pi(floor(pn/2)) */
    idx_t half = (idx_t)(pn_val / 2);   /* floor division */
    if (half > sieve_max) {
        /* enlarge sieve if necessary (unlikely for typical n) */
        ensure_sieve(half);
    }
    idx_t pi_half = pi_prefix_arr[half];

    /* result = (n - 1) - pi_half */
    idx_t res = (idx_t)(n - 1) - pi_half;
    return (seq_t)res;
}
