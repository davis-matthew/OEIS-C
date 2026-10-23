/*
 * A166663  Totally multiplicative sequence with a(p) = 5p+1 for prime p.
 * https://oeis.org/A166663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} (5p+1)^e (totally multiplicative, a(p) = 5p+1)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; values for n ≥ 1 are defined. */
    if (n == 0)
        return (seq_t)0;          /* safety guard – outside the defined range */

    /* n=1 is the identity element */
    if (n == 1)
        return (seq_t)1;

    /* Factor n into primes and exponents. The product of distinct primes
       dividing a 64‑bit integer fits easily into a small array. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        size_t   exp = (size_t)exps[i];

        /* Compute (5p + 1) as a seq_t to avoid overflow of uint64_t. */
        seq_t base = (seq_t)5 * (seq_t)p + (seq_t)1;

        /* Raise base to the exponent exp. */
        seq_t term = (seq_t)1;
        for (size_t k = 0; k < exp; ++k)
            term *= base;

        result *= term;
    }

    return result;
}
