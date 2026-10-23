/*
 * A104638  Number of odd digits in n-th prime.
 * https://oeis.org/A104638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the number of odd decimal digits of the n‑th prime. The
 *            function obtains the nth prime via a simple Eratosthenes sieve
 *            (reused across calls) and counts its odd digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_odd_digits_uint64, estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/count_odd_digits_uint64.h"

/* Static storage for the primes found so far. */
static uint64_t *prime_buf = NULL; /* array used as seq_t values */
static size_t     prime_cnt = 0;   /* number of primes stored */

/* Ensure that prime_buf contains at least the first n primes. */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_cnt)
        return; /* already have enough primes */

    uint64_t limit = estimate_prime_upper_bound(n);
    if (limit < 2u)
        limit = 2u;

    /* Keep doubling the limit until it contains at least n primes. */
    while (true) {
        /* Allocate and initialise sieve array. */
        uint8_t *is_prime = (uint8_t *)calloc((size_t)limit + 1, 1);
        if (!is_prime)
            return;                      /* allocation failure – give up */

        for (uint64_t i = 2; i <= limit; ++i)
            is_prime[i] = 1u;

        /* Eratosthenes sieve. */
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0u;
            }
        }

        /* Count primes up to limit. */
        size_t cnt = 0;
        for (uint64_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                ++cnt;

        if (cnt < (size_t)n) {          /* not enough – increase limit */
            free(is_prime);
            limit *= 2u;
            continue;
        }

        /* Allocate/reallocate storage for prime_buf. */
        uint64_t *new_buf = (uint64_t *)realloc(prime_buf,
                                                cnt * sizeof(uint64_t));
        if (!new_buf) {                    /* allocation failure – give up */
            free(is_prime);
            return;
        }
        prime_buf = new_buf;
        prime_cnt = cnt;

        /* Populate the prime array. */
        size_t idx = 0;
        for (uint64_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                prime_buf[idx++] = i;
            }
        }

        free(is_prime);
        break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined offset, return 0 */

    ensure_primes(n);

    if ((size_t)n > prime_cnt)   /* safety check */
        return (seq_t)0;

    /* Prime pigeon at index n-1 (since array is 0‑based). */
    uint64_t p = prime_buf[(size_t)n - 1];
    int odd = count_odd_digits_uint64(p);
    return (seq_t)odd;
}
