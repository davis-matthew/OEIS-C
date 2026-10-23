/*
 * A140200  Partial sums of A140080.
 * https://oeis.org/A140200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σₖ₌₁ⁿ ((k%3==0)?0:((popcount(k)==2)?2:1))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   popcount_le2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/popcount_le2.h"
/* Where popcount(k) is the number of 1‑bits in the binary expansion of k. */
/* This implements OEIS A140200 – the partial sums of A140080. */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if ((i % 3) == 0) {
            continue;            /* contributes 0 */
        }

        int pc = popcount_le2(i);
        seq_t inc = (pc == 2) ? (seq_t)2 : (seq_t)1;
        sum += inc;
    }
    return sum;
}
