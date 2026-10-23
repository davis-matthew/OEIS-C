/*
 * A031381  a(n) = prime(7*n - 2).
 * https://oeis.org/A031381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(7*n - 2) (1‑based index)
 * Range:     u8 n <= 8, u16 n <= 934, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static state for prime generation */
static idx_t   max_limit   = 0;
static uint8_t *is_composite = NULL;
static size_t  composite_size = 0;
static idx_t  *prime_array   = NULL;
static size_t  prime_count    = 0;

/* A simple upper bound for the k‑th prime: p_k < 15*k + 10  (valid for all k ≥ 1) */
static idx_t _prime_upper_bound(idx_t k)
{
    if (k == 0) return 1;
    return (idx_t)(15 * k + 10);
}

/* Ensure that at least k primes are available in prime_array */
static void _ensure_primes(idx_t k)
{
    if (prime_count >= (size_t)k) return;

    idx_t limit = _prime_upper_bound(k);
    if (limit <= max_limit) {
        /* Should not happen because prime_count < k implies limit > max_limit,
           but keep the guard safe. */
        limit = max_limit + 1;
    }

    /* Reallocate is_composite to new limit */
    uint8_t *tmp = (uint8_t *)realloc(is_composite, (size_t)limit + 1);
    if (!tmp) return;                /* allocation failure – leave state unchanged */
    is_composite   = tmp;
    composite_size = (size_t)limit + 1;

    /* Initialise new entries to non‑composite (0) */
    for (size_t i = 0; i <= (size_t)limit; ++i) is_composite[i] = 0;
    is_composite[0] = 1;
    is_composite[1] = 1;

    /* Sieve of Eratosthenes up to 'limit' */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* Count primes up to 'limit' */
    size_t new_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) ++new_count;
    }

    /* Reallocate prime_array to hold the new primes */
    idx_t *tmp_primes = (idx_t *)realloc(prime_array, new_count * sizeof(idx_t));
    if (!tmp_primes) return;       /* allocation failure */
    prime_array = tmp_primes;

    /* Fill the prime array */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            prime_array[pos++] = i;
        }
    }

    prime_count = new_count;
    max_limit   = limit;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    idx_t k = 7 * n - 2;
    if (k < 1) k = 1;      /* safety for n = 0 (never used) */

    _ensure_primes(k);
    if (k > (idx_t)prime_count) return 0;  /* safety */

    return (seq_t)prime_array[k - 1];
}
