/*
 * A175433  a(n) = the smallest number m such that sigma(n) = m^k for any k >=
 *          1 (sigma = A000203).
 * https://oeis.org/A175433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest m such that σ(n) = m^k for some k ≥ 1 (σ =
 *            A000203, sum‑of‑divisors function)
 * Range:     u8 n <= 107, u16 n <= 9340, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   factor_u64, gcd_int, seq_mul, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/gcd_int.h"     /* int gcd_int(int a, int b); */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* compute σ(n) */
    seq_t sigma = sigma_of(n);
    if (sigma <= 1)          /* σ(1) = 1 → a(1) = 1 */
        return sigma;

    /* σ(n) fits in 64‑bit for all n in the intended range */
    uint64_t s = (uint64_t)sigma;

    /* factor σ(n) */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(s, primes, exps, &cnt);
    if (cnt == 0)            /* should not occur */
        return sigma;

    /* compute gcd of the exponents */
    int g = (int)exps[0];
    for (size_t i = 1; i < cnt; ++i)
        g = gcd_int(g, (int)exps[i]);

    /* build m = ∏ p^(e/g) */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e_div_g = (idx_t)(exps[i] / g);
        seq_t term = seq_pow((seq_t)primes[i], e_div_g);
        result = seq_mul(result, term);
    }
    return result;
}
