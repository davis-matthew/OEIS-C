/*
 * A063475  a(n) = Sum_{d | H(n)} d^2, where H(n) is the half-totient function
 *          (A023022).
 * https://oeis.org/A063475
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σ_{d | H(n)} d² where H(n) = #{1 ≤ k ≤ n/2 : gcd(k,n)=1}
 *            – the half‑totient function. The function below implements this
 *            directly using inclusion–exclusion for H(n) and a simple divisor
 *            enumeration for the sum of squares
 * Range:     u8 n <= 30, u16 n <= 432, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   half_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/half_totient.h"

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* offset is 3; behaviour below is irrelevant */

    uint64_t H = half_totient((uint64_t)n);  /* H(n) fits easily in 64‑bit */

    /* Sum of squares of all divisors of H. */
    seq_t sum = 0;
    for (idx_t d = 1; (uint64_t)d * (uint64_t)d <= H; ++d)
    {
        if (H % d == 0)
        {
            uint64_t d_sq = (uint64_t)d * (uint64_t)d;
            uint64_t other = H / d;
            if (other == d)              /* perfect square case */
            {
                sum += (seq_t)d_sq;
            }
            else
            {
                sum += (seq_t)d_sq;
                sum += (seq_t)(other * other);
            }
        }
    }
    return sum;
}
