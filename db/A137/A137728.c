/*
 * A137728  Second digit from the end of product of first n primes.
 * https://oeis.org/A137728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A002110(n) / 10) mod 10 for n > 2; a(1) = a(2) = 0. The
 *            value is the second‑to‑last digit of the primorial
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Base cases as defined in OEIS */
    if (n <= 2)
        return 0;

    /* Obtain the first n primes */
    uint32_t *primes = NULL;
    size_t count = 0;
    sieve_first_n_primes(n, &primes, &count);

    /* Compute product of the primes modulo 100.
       We use a 32‑bit intermediate to avoid overflow on small seq_t types. */
    seq_t prod = 1;
    for (size_t i = 0; i < count; ++i)
    {
        uint32_t prime_mod = primes[i] % 100;
        uint32_t temp = (uint32_t)prod * prime_mod;
        prod = (seq_t)(temp % 100);   /* prod now fits in seq_t (0..99) */
    }

    /* The primorial ends with exactly one zero for n ≥ 3.
       Thus prod = 10 * d where d is the desired digit. */
    free(primes);
    return prod / 10;
}
