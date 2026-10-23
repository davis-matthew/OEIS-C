/*
 * A193335  Number of odd divisors of sigma(n).
 * https://oeis.org/A193335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001227(A000203(n)) = number of odd divisors of σ(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute σ(n) via prime factorisation */
    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(m, primes, exps, &pcnt);

    uint64_t sigma_val = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t term = 1;
        uint64_t power = 1;
        for (uint64_t k = 0; k < e; ++k) {
            power *= p;
            term += power;                /* 1 + p + p² + … + pᵉ */
        }
        sigma_val *= term;                /* multiply by factor for p */
    }

    /* Remove all powers of two to obtain the odd part of σ(n) */
    uint64_t odd = sigma_val;
    while ((odd & 1ULL) == 0 && odd > 0) {
        odd >>= 1;
    }

    /* Return the number of divisors of the odd part */
    return (seq_t)divisor_count_u64(odd);
}
