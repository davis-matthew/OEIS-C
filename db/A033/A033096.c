/*
 * A033096  Number of 1's when k is written in base b for all b and k
 *          satisfying 2<=b<=n+1, 1<=k<=n.
 * https://oeis.org/A033096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{k=1}^n ∑_{b=2}^{k+1} (number of 1's in the base‑b
 *            representation of k)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   sum_ones_in_base_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_ones_in_base_upto.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t b = 2; b <= n + 1; ++b)
    {
        seq_t upto_n   = sum_ones_in_base_upto(b, n);
        seq_t upto_low = 0;
        if (b >= 3)
            upto_low = sum_ones_in_base_upto(b, b - 2);
        total += (upto_n - upto_low);
    }
    return total;
}
