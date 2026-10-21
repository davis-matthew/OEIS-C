/*
 * A082903  Highest power of two that divides the sum of divisors of n.
 * https://oeis.org/A082903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^v₂(σ(n)) where σ(n) is the sum of divisors of n
 * Range:     u8 n <= 18, u16 n <= 2083, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factor n (using the provided factor_u64 helper), form each *odd* (p^e+1-1)/(p-1) term, multiply all terms to get σ(n), and then count trailing 2‑factors. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case: n = 0 is outside the sequence domain, but treat it as 1 for safety. */
    if (n == 0) return 1;

    /* Factor the integer n. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute σ(n) = ∏ (p^(e+1)-1)/(p-1). */
    seq_t sigma = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = 1;
        seq_t p = (seq_t)primes[i];

        /* Compute p^(e+1). */
        for (unsigned j = 0; j < exps[i] + 1; ++j) {
            term *= p;
        }

        /* term = (p^(e+1)-1)/(p-1). */
        term = (term - 1) / (p - 1);

        sigma *= term;
    }

    /* Count trailing zeros in sigma (i.e., exponent of 2). */
    unsigned v = 0;
    seq_t temp = sigma;
    while ((temp & 1) == 0) {
        ++v;
        temp >>= 1;
    }

    return (seq_t)1 << v;
}
