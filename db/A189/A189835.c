/*
 * A189835  Number of representations of n as a*b + b*c + c*d + d*e where a,
 *          b, d, e>0, c>=0 are integers.
 * https://oeis.org/A189835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d^2 – n·τ(n) (σ₂(n) – n·τ(n))
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Trial‑division factorisation of n (idx_t).  Works for any n ≥ 1. */
seq_t a(idx_t n)
{
    idx_t tmp = n;
    seq_t sigma2 = 1;   /* product of σ₂(pᵉ) for each prime power */
    seq_t tau     = 1;   /* product of (e + 1)                           */

    /* handle factor 2 separately for speed */
    if ((tmp & 1) == 0) {
        idx_t e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        seq_t p = 2;
        seq_t p_sq = p * p;               /* 4 */
        seq_t pow_sq = 1;
        for (idx_t i = 0; i < e + 1; ++i) /* (p²)^(e+1) = p²(e+1)    */
            pow_sq *= p_sq;
        seq_t sigma_p = (pow_sq - 1) / (p_sq - 1); /* σ₂(pᵉ)           */
        sigma2 *= sigma_p;
        tau     *= (e + 1);
    }

    /* odd factors */
    for (idx_t d = 3; d <= tmp / d; d += 2) {
        if (tmp % d == 0) {
            idx_t e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }
            seq_t p = d;
            seq_t p_sq = p * p;
            seq_t pow_sq = 1;
            for (idx_t i = 0; i < e + 1; ++i)
                pow_sq *= p_sq;
            seq_t sigma_p = (pow_sq - 1) / (p_sq - 1);
            sigma2 *= sigma_p;
            tau     *= (e + 1);
        }
    }

    /* remainder is prime (if > 1) */
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;              /* prime leftover */
        seq_t sigma_p = 1 + p * p;         /* σ₂(p¹) = 1 + p²     */
        sigma2 *= sigma_p;
        tau   *= 2;                        /* (1 + 1)              */
    }

    return sigma2 - (seq_t)n * tau;
}
