/*
 * A119616  Second elementary symmetric function of divisors of n.
 * https://oeis.org/A119616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (σ₁(n)² - σ₂(n)) / 2, where σ_k(n) = sum_{d|n} d^k
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 3155, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided factorizer and integer‑power routine.
   It works for all builds (fixed‑size unsigned, big integers) and respects
   the required function signature.   */

#include <stdint.h>
#include "../utils/factor_u64.h"       /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/pow_int.h"         /* pow_int(seq_t, idx_t) */

seq_t a(idx_t n)
{
    /* n is at least 1 for valid calls; guard against 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* factor_u64 works with uint64_t inputs */
    uint64_t nn = (uint64_t)n;

    /* Allocate buffers for prime factors and exponents */
    /* 64 distinct prime factors is more than enough for n <= 10¹⁴  */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t sigma1 = (seq_t)1;   /* σ₁ starts at product of 1 */
    seq_t sigma2 = (seq_t)1;   /* σ₂ starts at product of 1 */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)(exps[i]);

        /* σ₁ contribution: (p^(e+1) - 1)/(p - 1) */
        seq_t p_pow1 = pow_int(p, e + 1);
        seq_t term1  = (p_pow1 - (seq_t)1) / (p - (seq_t)1);
        sigma1 *= term1;

        /* σ₂ contribution: (p^(2(e+1)) - 1)/(p² - 1) */
        seq_t p_pow2 = pow_int(p, 2 * (e + 1));
        seq_t denom2 = p * p - (seq_t)1;
        seq_t term2  = (p_pow2 - (seq_t)1) / denom2;
        sigma2 *= term2;
    }

    /* Final formula: (σ1² - σ2)/2 */
    seq_t result = (sigma1 * sigma1 - sigma2) / ((seq_t)2);
    return result;
}
