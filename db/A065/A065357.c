/*
 * A065357  a(n) = (-1)^pi(n) where pi(n) is the number of primes <= n.
 * https://oeis.org/A065357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, changed
 * Formula:   a(n) = (-1)^pi(n) where pi(n) counts primes <= n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static void generate_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Allocate sieve array. Limit is known to be small (e.g., < 10000). */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Initialize. */
    size_t i;
    for (i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[(size_t)p]) {
            for (idx_t j = p * p; j <= limit; j += p) {
                is_prime[(size_t)j] = 0;
            }
        }
    }

    /* Count primes. */
    size_t count = 0;
    for (i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) ++count;
    }

    /* Allocate array of primes and fill it. */
    uint64_t *pr = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            pr[idx++] = i;
        }
    }

    free(is_prime);
    *primes_out = pr;
    *cnt_out = count;
}

seq_t a(idx_t n)
{
    static uint64_t *cached_primes = NULL;
    static size_t cached_cnt = 0;
    static idx_t cached_limit = 0;

    if (n > cached_limit) {
        free(cached_primes);
        generate_primes(n, &cached_primes, &cached_cnt);
        cached_limit = n;
    }
    return (seq_t)((cached_cnt & 1) ? -1 : 1);
}
