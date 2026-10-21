/*
 * A227738  Irregular table read by rows: each row n (n>=1) lists the
 *          positions where the runs of bits change between 0's and 1's in the
 *          binary expansion of n, when scanning it from the least significant
 *          to the most significant end.
 * https://oeis.org/A227738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is the concatenation of the positions of the 1‑bits in the
 *            binary expansion of the Gray code of each integer i starting
 *            with i=1. That is, for the smallest m with S(m−1) < n ≤ S(m),
 *            S(t)=Σ_{j=1}^{t} popcount(Gray(j)), the required term is the
 *            (n−S(m−1))‑th 1‑bit (counting from least signific...
 * Range:     u8 n <= 5120, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used for the OEIS offset 1 */

    idx_t total = 0;          /* cumulative number of terms produced so far */
    for (idx_t i = 1; ; ++i)
    {
        /* Gray code of i */
        idx_t gray = i ^ (i >> 1);

        /* number of set bits in Gray(i) */
        uint64_t ones = __builtin_popcountll(gray);

        /* check whether the desired term lies in this row */
        if (total + ones >= n)
        {
            /* offset within this row, zero‑based */
            idx_t offset = n - total - 1;

            /* find the offset‑th set bit in gray, counting from LSB */
            idx_t pos = 1;               /* 1‑based position */
            while (gray)
            {
                if (gray & 1ULL)
                {
                    if (offset == 0)
                        return (seq_t)pos;
                    --offset;
                }
                gray >>= 1;
                ++pos;
            }
            /* should never reach here */
            return (seq_t)0;
        }

        total += ones;
    }
}
