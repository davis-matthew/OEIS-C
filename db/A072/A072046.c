/*
 * A072046  Greatest common divisor of product of divisors of n and product of
 *          non-divisors < n.
 * https://oeis.org/A072046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = GCD( n^{τ/2}, (n-1)! / n^{τ/2 - 1} ) where τ = ∏(e_i + 1)
 *            for n = ∏p_i^{e_i}
 * Range:     u8 n <= 19, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;  /* product of divisors = 1, (n‑1)! = 1 */

    /* prime factors of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* number of divisors τ */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (exps[i] + 1);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        idx_t e     = exps[i];
        idx_t p     = (idx_t)primes[i];

        /* exponent of p in product of all divisors */
        idx_t expG = e * tau / 2;  /* integer division: e*tau is even */

        /* exponent of p in (n-1)! */
        idx_t vp = 0;
        idx_t t  = n - 1;
        while (t) {
            t /= p;
            vp += t;
        }

        /* exponent of p in denominator n^{τ/2 - 1} */
        idx_t den_exp = e * (tau - 2) / 2;  /* τ/2 - 1 = (τ-2)/2 */

        /* exponent of p in the product of non‑divisors */
        idx_t expX = (vp > den_exp) ? (vp - den_exp) : 0;

        /* exponent in the GCD */
        idx_t gcd_exp = (expG < expX) ? expG : expX;
        if (gcd_exp == 0) continue;

        seq_t pow_val = seq_pow((seq_t)p, gcd_exp);
        result        = seq_mul(result, pow_val);
    }

    return result;
}
