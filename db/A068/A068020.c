/*
 * A068020  a(n) = Z(S_m; sigma[1](n), sigma[2](n),..., sigma[m](n)) where
 *          Z(S_m; x_1,x_2,...,x_m) is the cycle index of the symmetric group
 *          S_m and sigma[k](n) is the sum of k-th powers of divisors of n;
 *          m=3.
 * https://oeis.org/A068020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 839, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (σ₁(n)³ + 3·σ₁(n)·σ₂(n) + 2·σ₃(n)) / 6
 * where σₖ(n) is the sum of k‑th powers of the divisors of n.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is defined for n ≥ 1 */
        return 0;

    seq_t sigma1 = 1;          /* σ₁(1) = 1 */
    seq_t sigma2 = 1;          /* σ₂(1) = 1 */
    seq_t sigma3 = 1;          /* σ₃(1) = 1 */

    idx_t m = n;

    /* Prime factorisation of n by trial division. */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* Compute Σ_{k=0..exp} p^k , Σ p^{2k} , Σ p^{3k} for this prime power. */
        seq_t sum1 = 1,  /* σ₁(pᵉ) part */
              sum2 = 1,  /* σ₂(pᵉ) part */
              sum3 = 1;  /* σ₃(pᵉ) part */
        seq_t p_pow = 1;           /* current p^k */

        for (int k = 1; k <= exp; ++k) {
            p_pow *= (seq_t)p;              /* p^k */
            sum1 += p_pow;                  /* add p^k to σ₁ */

            seq_t p_pow2 = p_pow * p_pow;    /* p^(2k) */
            sum2 += p_pow2;                  /* add p^(2k) to σ₂ */

            seq_t p_pow3 = p_pow2 * p_pow;   /* p^(3k) */
            sum3 += p_pow3;                  /* add p^(3k) to σ₃ */
        }

        sigma1 *= sum1;
        sigma2 *= sum2;
        sigma3 *= sum3;
    }

    /* Remaining prime factor larger than sqrt(n). It appears only once. */
    if (m > 1) {
        seq_t p = (seq_t)m;
        seq_t sum1 = 1 + p;
        seq_t sum2 = 1 + p * p;
        seq_t sum3 = 1 + p * p * p;

        sigma1 *= sum1;
        sigma2 *= sum2;
        sigma3 *= sum3;
    }

    /* Compute the numerator of the formula. */
    seq_t term = sigma1 * sigma1 * sigma1;          /* σ₁³ */
    term += sigma1 * sigma2 * 3;                    /* 3·σ₁·σ₂ */
    term += sigma3 * 2;                             /* 2·σ₃ */

    /* Division by 6 is exact for all n. */
    return term / 6;
}
