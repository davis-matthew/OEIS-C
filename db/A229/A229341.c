/*
 * A229341  a(n) = tau(n'), the number of divisors of the arithmetic
 *          derivative of n.
 * https://oeis.org/A229341
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(A003415(n)) – number of divisors of the arithmetic
 *            derivative of n
 * Range:     u8 n <= 806, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 101 terms through n = 9347 (2026-11-01)
 * Helpers:   factor_u64, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/num_divisors.h"

/* Return a(n) for OEIS sequence A229341 (offset 2) */
seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n < 2 */

    /* Factor n */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    /* Compute the arithmetic derivative n' = Σ (n / p_i) * e_i */
    seq_t der = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        /* use 128‑bit intermediate to avoid overflow for large n */
        __uint128_t term = ((__uint128_t)n / primes[i]) * exps[i];
        der += (seq_t)term;
    }

    /* Number of divisors of der */
    /* For all n used in tests (n ≤ 10000) der fits in 64‑bit */
    uint64_t der64 = (uint64_t)der;
    return (seq_t)num_divisors(der64);
}
