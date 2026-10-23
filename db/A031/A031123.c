/*
 * A031123  Expansion of Sum_{m>=1} z^(m^2)/(1-z^((m+1)^2)).
 * https://oeis.org/A031123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { m ≥ 1 | n ≥ m^2 and (n - m^2) is divisible by (m+1)^2
 *            }
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the coefficient of z^n in  ∑_{m≥1} z^(m^2) / (1 - z^((m+1)^2)) */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t m = 1; m * m <= n; ++m)
    {
        idx_t m_sq = m * m;
        idx_t base = m + 1;
        idx_t base_sq = base * base;
        seq_t rem = ((seq_t)(n - m_sq)) % (seq_t)base_sq;
        if (rem == 0)
            ++count;
    }
    return count;
}
