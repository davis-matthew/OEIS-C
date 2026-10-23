/*
 * A048065  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(L,⌊(n−L)/4⌋)} C(L, k)·C(n−L, 4k) where L =
 *            ⌊(n−4)/2⌋. Each term counts subsets having exactly k elements ≤
 *            L and 4k elements > L. Only non‑empty subsets are counted
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* safety, offset starts at 1 */

    /* Compute L = ⌊(n−4)/2⌋ safely for unsigned n */
    idx_t L = (n >= 4) ? (n - 4) / 2 : 0;

    /* Remaining elements = n−L */
    idx_t remaining = n - L;

    /* k is limited by both L and remaining/4 */
    idx_t maxk = L < (remaining / 4) ? L : (remaining / 4);

    seq_t ans = 0;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t part1 = binom(L, k);
        seq_t part2 = binom(remaining, 4 * k);
        ans += part1 * part2;
    }
    return ans;
}
