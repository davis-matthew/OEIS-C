/*
 * A068037  Number of subsets of {1,2,3,...,n} that sum to 0 mod 16.
 * https://oeis.org/A068037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n ≤ 5, a(6) = 4, a(7) = 9, a(n) = 2^(n-4) for n ≥ 8
 *            (equivalently a(n) = 2^(-4+n) for n > 7). The function computes
 *            the values directly for n ≤ 7 by brute force over subsets, and
 *            applies the closed‑form formula for larger n
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 0 ≤ n ≤ 7 – brute force; 2⁷ = 128 iterations */
    if (n <= 7) {
        seq_t count = 0;
        idx_t total = ((idx_t)1) << n;   /* total number of subsets */
        for (idx_t mask = 0; mask < total; ++mask) {
            uint32_t sum = 0;
            for (idx_t i = 0; i < n; ++i) {
                if (mask & (((idx_t)1) << i))
                    sum += (uint32_t)(i + 1);
            }
            if ((sum % 16u) == 0)
                ++count;
        }
        return count;
    }

    /* n ≥ 8 – use the closed‑form a(n) = 2^(n-4) */
    return pow2(n - 4);
}
