/*
 * A172191  a(n) = 2*prime(n)^4.
 * https://oeis.org/A172191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n)^4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        /* sequence is defined from index 1; behaviour below offset is irrelevant */
        return 0;
    }

    /* Cache the first n primes for fast repeated access */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;

    /* Extend the cache if the requested index exceeds the current amount */
    if ((size_t)n > primes_cnt) {
        free(primes);  /* free previous buffer if any */
        sieve_first_n_primes(n, &primes, &primes_cnt);
    }

    /* Retrieve the nth prime (1‑based index) */
    uint32_t p = primes[n - 1];

    /* Compute 2 * p^4 using the arithmetic type seq_t */
    seq_t sp = (seq_t)p;
    seq_t pow4 = sp * sp * sp * sp;
    return (seq_t)2 * pow4;
}
