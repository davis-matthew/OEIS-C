/*
 * A030004  a(n) = (prime(n) - 1)*(prime(n) - 3)*(prime(n) - 5)/48.
 * https://oeis.org/A030004
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n) - 1)*(prime(n) - 3)*(prime(n) - 5)/48
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 777, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Cache the first n primes (1-indexed) */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;

    if ((size_t)n > primes_cnt)
    {
        /* Free previously allocated primes array */
        free(primes);
        size_t cnt;
        sieve_first_n_primes((idx_t)n, &primes, &cnt);
        primes_cnt = cnt; /* should equal n */
    }

    /* Retrieve the nth prime (array is 0‑based) */
    size_t idx = (size_t)(n - 1);
    uint64_t p = primes[idx];

    /* Compute the formula using 64‑bit intermediate */
    uint64_t prod = (p - 1ULL) * (p - 3ULL) * (p - 5ULL);
    prod /= 48ULL;

    return (seq_t)prod;
}
