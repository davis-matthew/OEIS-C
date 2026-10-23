/*
 * A038995  Number of sublattices of index n in generic 8-dimensional lattice.
 * https://oeis.org/A038995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∣∣n} ∏_{k=1..7} (p^(e+k)−1)/(p^k−1)
 *            (multiplicative, Q = 8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 455, u128 n <= 6113,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Return the number of sublattices of index n in a generic 8‑dimensional lattice. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; this value is never used */

    /* Factor the integer n (uint64_t is sufficient for the tests). */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* Compute a(p^e) = ∏_{k=1..7} (p^(e+k)−1)/(p^k−1). */
        seq_t prime_term = 1;
        for (int k = 1; k <= 7; ++k) {
            seq_t num   = big_pow(p, (seq_t)(e + k)) - (seq_t)1;
            seq_t denom = big_pow(p, (seq_t)k) - (seq_t)1;
            prime_term = (prime_term * num) / denom;   /* exact division */
        }
        result *= prime_term;
    }
    return result;
}
