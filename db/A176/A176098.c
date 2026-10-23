/*
 * A176098  a(n) = prime(n) times the n-th nonnegative noncomposite.
 * https://oeis.org/A176098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime(n) * A158611(n) where * A158611(1)=0, A158611(2)=1,
 *            and for n≥3, A158611(n)=prime(n‑2)
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Static prime table */
static uint64_t *primes = NULL;
static size_t     primes_count = 0;

/* Ensure that at least |n| primes are stored (indexing from 1). */
static void extend_primes_to(idx_t n)
{
    if (primes_count >= (size_t)n)
        return;

    /* Rough upper bound for the n-th prime: n*15 is enough for n≤10000. */
    idx_t limit = (n < 6) ? 15 : n * 15;

    /* Allocate sieve array */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime)
        abort();

    for (size_t i = 0; i <= (size_t)limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[(size_t)p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[(size_t)q] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    /* Allocate and fill prime array */
    uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!arr) {
        free(is_prime);
        abort();
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            arr[pos++] = (uint64_t)i;

    /* Replace old prime table */
    if (primes)
        free(primes);
    primes = arr;
    primes_count = cnt;

    free(is_prime);
}

/* seq_t a(idx_t n) – compute the n-th term of A176098 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                      /* undefined for n<1 */

    if (n == 1)
        return (seq_t)0;                      /* A158611(1) = 0 */

    /* Ensure primes up to n are available */
    extend_primes_to(n);

    seq_t prime_n = (seq_t)primes[(size_t)n - 1];   /* prime(n)   */
    seq_t noncom  = (n == 2) ? (seq_t)1            /* A158611(2) */
                             : (seq_t)primes[(size_t)n - 3]; /* prime(n-2) */

    return prime_n * noncom;                     /* product */
}
