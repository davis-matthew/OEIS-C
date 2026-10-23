/*
 * A037164  Numerators of coefficients of Eisenstein series E_12(q) (or E_6(q)
 *          or E_24(q)).
 * https://oeis.org/A037164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 65520 * σ₁₁(n) for n ≥ 1, a(0) = 1 where σ₁₁(n) = Σ_{d|n}
 *            d¹¹ = ∏_{p^e‖n} (1 + p¹¹ + p²¹¹ + … + p^{e·11})
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 20, u128 n <= 1161,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* Computes a(n) for the OEIS sequence A037164. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* constant term of the Eisenstein series */

    /* Factorise n into primes: n = ∏ pᵉ */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    const unsigned k = 11;                /* exponent used in the series */
    seq_t sigma = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* Compute p^k once; it fits in 64 bits for all p ≤ 10⁴. */
        seq_t pk = seq_pow((seq_t)p, (idx_t)k);

        /* Compute the factor (1 + p^k + p^{2k} + … + p^{e·k}) */
        seq_t factor = (seq_t)1;
        seq_t term   = (seq_t)1;
        for (idx_t j = 1; j <= (idx_t)e; ++j) {
            term   = term * pk;
            factor = factor + term;
        }

        sigma = sigma * factor;
    }

    /* Multiply by the constant 65520 */
    const seq_t coef = (seq_t)65520;
    return sigma * coef;
}
