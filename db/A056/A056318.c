/*
 * A056318  Number of primitive (aperiodic) reversible strings with n beads
 *          using exactly two different colors.
 * https://oeis.org/A056318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * ((2^{n/d} + 2^{⌈(n/d)/2⌉}) / 2), with a(1)
 *            = 0. μ is the Möbius function. The inner term counts all
 *            reversible binary strings of length n/d (palindromes are handled
 *            by the ceil exponent)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
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
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d)
            continue;

        int mu = mu_of(d);
        if (mu != 0)
        {
            idx_t t = n / d;
            seq_t val = (ipow_u64_idx(2ULL, t) +
                         ipow_u64_idx(2ULL, (t + 1) / 2)) / 2;
            if (mu > 0)
                sum += val;
            else
                sum -= val;
        }

        idx_t d2 = n / d;
        if (d2 != d)
        {
            mu = mu_of(d2);
            if (mu != 0)
            {
                idx_t t = n / d2; /* t = d */
                seq_t val = (ipow_u64_idx(2ULL, t) +
                             ipow_u64_idx(2ULL, (t + 1) / 2)) / 2;
                if (mu > 0)
                    sum += val;
                else
                    sum -= val;
            }
        }
    }

    return sum;
}
