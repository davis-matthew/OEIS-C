/*
 * A191750  Dirichlet convolution of A000012 with A007947.
 * https://oeis.org/A191750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (1 + p·e) (multiplicative definition)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Arrays large enough for any 64‑bit number */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n: primes[i] holds the i‑th prime factor,
       exps[i] the corresponding exponent. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;          /* multiplicative identity */
    for (size_t i = 0; i < cnt; ++i)
    {
        /* Each local factor is 1 + p*e */
        seq_t term = 1 + (seq_t)primes[i] * (seq_t)exps[i];
        result *= term;
    }
    return result;
}
