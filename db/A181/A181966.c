/*
 * A181966  Sum of the sizes of normalizers of all prime order cyclic
 *          subgroups of the symmetric group S_n.
 * https://oeis.org/A181966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{p prime ≤ n} ⌊ n / p ⌋
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached data for primes */
static size_t cached_limit = 0;
static bool *prime_flag = NULL;
static uint32_t *prime_list = NULL;  /* stores primes up to cached_limit */
static size_t prime_count = 0;

/* Ensure primes up to at least n are available */
static void
ensure_primes(idx_t n)
{
    if (n <= (idx_t)cached_limit)
        return;

    /* Release old data */
    free(prime_flag);
    free(prime_list);

    /* Allocate new sieve array */
    size_t limit = (size_t)n;
    prime_flag = (bool *)calloc(limit + 1, sizeof(bool));
    if (!prime_flag)
        abort();

    /* Sieve of Eratosthenes */
    if (limit >= 2) {
        prime_flag[0] = prime_flag[1] = false;
        for (size_t i = 2; i <= limit; ++i)
            prime_flag[i] = true;
        for (size_t i = 2; (idx_t)i * i <= n; ++i) {
            if (prime_flag[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    prime_flag[j] = false;
            }
        }
    }

    /* Allocate room for prime list (at most limit/2 primes) */
    prime_list = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!prime_list)
        abort();

    /* Collect primes into the list */
    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) {
            prime_list[prime_count++] = (uint32_t)i;
        }

    cached_limit = limit;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* safety: sequence is defined for n ≥ 1 */

    /* Prepare prime data */
    ensure_primes(n);

    /* Compute sum_{p prime ≤ n} ⌊ n / p ⌋ */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = (idx_t)prime_list[i];
        if (p > n)
            break;
        sum += (seq_t)(n / p);
    }

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Return product */
    return fact * sum;
}
