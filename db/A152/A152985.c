/*
 * A152985  Sum of proper divisors minus the number of proper divisors of the
 *          square A000290(n).
 * https://oeis.org/A152985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = σ(n^2) – n^2 – τ(n^2) + 1 where σ is the sum of divisors
 *            and τ the divisor count
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 5637, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factorise n (up to 2^64‑1). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute σ(n^2) and τ(n^2). */
    seq_t sigma = 1;      /* Σ factors   */
    seq_t tau   = 1;      /* τ factors   */
    seq_t n2    = (seq_t)n * (seq_t)n;   /* n^2           */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t exp = 2 * e + 1;  /* exponent of p in n^2 plus one */

        seq_t pow_p = 1;
        for (size_t j = 0; j < exp; ++j)
            pow_p *= (seq_t)p;          /* p^(2e+1) */

        seq_t numer = pow_p - 1;
        seq_t denom = (seq_t)p - 1;    /* divides exactly */

        sigma = sigma * numer / denom; /* σ(n^2) */
        tau   = tau * (seq_t)(2 * e + 1); /* τ(n^2) */
    }

    return sigma - n2 - tau + 1;
}
