/*
 * A236227  Sum of the nineteenth powers of the first n primes.
 * https://oeis.org/A236227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum(k = 1 .. n, prime(k)^19)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 4, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/pow_uint.h"

/* Computes the sum of the 19th powers of the first n primes. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Get the first n primes. */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);

    /* If the sieve failed, return 0. */
    if (cnt != (size_t)n) {
        free(primes);
        return (seq_t)0;
    }

    /* Accumulate sum in the type seq_t, which handles
       the necessary bit‑width (standard 64‑bit or arbitrary
       precision). */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pwr = pow_uint(primes[i], 19);
        sum += pwr;
    }

    free(primes);
    return sum;
}
