/*
 * A076577  Sum of squares of divisors d of n such that n/d is odd.
 * https://oeis.org/A076577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = 4^v2(n) * σ₂( n / 2^v2(n) ) where v2(n) is the exponent
 *            of 2 in n and σ₂(k) is the sum of squares of all divisors of k
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 2304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64() */
#include "../utils/pow_seq_t.h"   /* pow_seq_t() */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused, keeps compiler happy */

    /* 1. Determine exponent of 2 in n */
    uint64_t tmp = (uint64_t)n;
    size_t e = 0;
    while ((tmp & 1ULL) == 0) {
        e++;
        tmp >>= 1;
    }
    uint64_t odd_part = tmp;        /* n / 2^e */

    /* 2. Factor odd_part (if ≠ 1) */
    uint64_t pr[32];
    uint64_t exp[32];
    size_t cnt = 0;
    if (odd_part > 1) {
        factor_u64(odd_part, pr, exp, &cnt);
    } else {
        cnt = 0;                    /* no odd prime factors */
    }

    /* 3. Compute σ₂(odd_part) */
    seq_t sigma = 1;                /* multiplicative identity */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)pr[i];
        int   k   = (int)exp[i];
        int   pow_exp = 2 * (k + 1);       /* exponent of p in numerator   */
        seq_t num = pow_seq_t(p, pow_exp);
        num -= 1;                         /* p^(2(k+1)) – 1 */

        seq_t den = pow_seq_t(p, 2);
        den -= 1;                         /* p^2 – 1 */

        seq_t term = num / den;           /* exact division */
        sigma *= term;
    }

    /* For odd_part == 1, sigma stays 1 (σ₂(1) = 1). */

    /* 4. Multiply by 4^e (i.e., (2^2)^e).  */
    for (size_t i = 0; i < e; ++i) {
        sigma *= (seq_t)4;
    }

    return sigma;                      /* a(n) */
}
