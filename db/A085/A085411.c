/*
 * A085411  Total number of parts in all compositions of n into relatively
 *          prime parts.
 * https://oeis.org/A085411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} μ(n/d)·(d+1)·2^(d-2) (with the convention
 *            (d+1)·2^(d-2) = 1 when d = 1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_u64_idx.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;   /* Σ of positive contributions */
    seq_t sum_neg = 0;   /* Σ of magnitudes of negative contributions */

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i != 0)
            continue;

        /* first divisor d = i */
        idx_t d = i;
        int mu = mu_of(n / d);
        if (mu != 0)
        {
            seq_t term;
            if (d == 1)
                term = 1;   /* (1+1)·2^(-1) → 1 by definition */
            else
                term = (seq_t)(d + 1) * ipow_u64_idx(2, d - 2);

            if (mu > 0)
                sum_pos += term;
            else
                sum_neg += term;
        }

        /* second divisor d2 = n / i, if distinct */
        idx_t d2 = n / i;
        if (d2 != d)
        {
            mu = mu_of(n / d2);
            if (mu != 0)
            {
                seq_t term;
                if (d2 == 1)
                    term = 1;
                else
                    term = (seq_t)(d2 + 1) * ipow_u64_idx(2, d2 - 2);

                if (mu > 0)
                    sum_pos += term;
                else
                    sum_neg += term;
            }
        }
    }

    /* For all n, Σ_{d|n} μ(n/d)·term ≥ 0, so subtraction is safe */
    return sum_pos - sum_neg;
}
