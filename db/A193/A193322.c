/*
 * A193322  Sum of even divisors of lambda(n).
 * https://oeis.org/A193322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of even divisors of λ(n) (Carmichael function)
 * Range:     u8 n <= 156, u16 n <= 9339, u32 n <= 9339, u64 n <= 9339,
 *            u128 n <= 9339, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"           /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n >= 1 */

    /* factor n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* compute lambda(n) = lcm of lambda(p^e) */
    seq_t lam = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t l;

        if (p == 2) {
            if (e == 1)        l = 1;
            else if (e == 2)   l = 2;
            else {
                seq_t pow = 1;
                for (uint64_t k = 0; k < e - 2; ++k) pow *= 2;
                l = pow;
            }
        } else {
            seq_t pow = 1;
            for (uint64_t k = 0; k < e - 1; ++k) pow *= p;
            l = pow * (p - 1);
        }

        seq_t g = gcd(lam, l);
        lam = (lam / g) * l;   /* lcm so far */
    }

    /* if λ(n) is odd, it has no even divisors */
    if (((uint64_t)lam & 1) == 1) return 0;

    /* factor λ(n) */
    uint64_t pr2[64];
    uint64_t ex2[64];
    size_t cnt2;
    factor_u64((uint64_t)lam, pr2, ex2, &cnt2);

    /* compute sum of all divisors (σ) and sum of odd divisors (σ_odd) */
    seq_t sigma = 1;
    seq_t odd_sigma = 1;
    for (size_t i = 0; i < cnt2; ++i) {
        uint64_t p = pr2[i];
        uint64_t e = ex2[i];

        /* compute sum_{k=0}^e p^k */
        seq_t sum = 1;
        seq_t pow = 1;
        for (uint64_t k = 0; k < e; ++k) {
            pow *= p;
            sum += pow;
        }

        sigma *= sum;
        if (p & 1) odd_sigma *= sum;   /* only odd primes */
    }

    /* sum of even divisors = σ - σ_odd (if λ(n) is even) */
    seq_t even_sigma = sigma - odd_sigma;
    return even_sigma;
}
