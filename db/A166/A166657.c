/*
 * A166657  Totally multiplicative sequence with a(p) = 8p-1 for prime p.
 * https://oeis.org/A166657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (8p - 1)^e (totally multiplicative)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses the provided factor_u64 helper to factor the
 * argument n into primes and exponents, then multiplies the
 * corresponding (8p-1)^e values in seq_t.  It handles n == 1
 * specially, returning 1.  All arithmetic is performed in seq_t
 * so the value is exact whenever seq_t can represent it. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* arrays large enough for all possible prime factors of n <= 2^64-1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* term = 8*p - 1 in seq_t to avoid 64-bit overflow */
        seq_t term = (seq_t)8 * (seq_t)primes[i] - 1;
        /* compute term^exps[i] by repeated multiplication */
        seq_t pow_val = 1;
        for (size_t j = 0; j < exps[i]; ++j) {
            pow_val *= term;
        }
        result *= pow_val;
    }
    return result;
}
