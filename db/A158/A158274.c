/*
 * A158274  Numerators of antiharmonic means of divisors of n.
 * https://oeis.org/A158274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   * a(n) = numerator of the reduced fraction σ₂(n)/σ₁(n), * where
 *            σ₁(n) = sum of divisors of n, * σ₂(n) = sum of squares of
 *            divisors of n. * Using the product formula: * σ₂(n)/σ₁(n) = ∏ₚ
 *            (p^(e+1)+1)/(p+1) for n = ∏ₚ p^e * The returned value is the
 *            numerator after cancelling common factors
 * Range:     u8 n <= 14, u16 n <= 325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/gcd.h"         /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/seq_one.h"     /* seq_t seq_one(void); */

seq_t a(idx_t n) {
    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t N = seq_one();   /* numerator product ∏(p^(e+1)+1) */
    seq_t D = seq_one();   /* denominator product ∏(p+1)     */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];

        /* compute p^(e+1) + 1 */
        seq_t num = seq_pow(p, e + 1);
        num += (seq_t)1;
        /* compute p + 1 */
        seq_t den = p + (seq_t)1;

        N *= num;
        D *= den;
    }

    /* cancel common factors */
    seq_t g = gcd(N, D);
    N /= g;

    return N;
}
