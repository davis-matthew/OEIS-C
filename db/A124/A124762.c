/*
 * A124762  Number of levels for compositions in standard order.
 * https://oeis.org/A124762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = Sum_{1 <= i < k, b(i)=b(i+1)} 1, where b is the
 *            composition of n in standard order (OEIS A124762)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    idx_t tmp_n = n;          /* working copy of n */
    idx_t bitpos = 1;         /* 1‑based bit position (LSB -> 1) */
    idx_t prevpos = 0;        /* previous set‑bit position */
    seq_t diff_prev = 0;      /* previous difference */
    seq_t count = 0;          /* result */
    bool first = true;        /* flag for first bit processed */

    while (tmp_n) {
        if (tmp_n & 1) {
            idx_t currpos = bitpos;
            seq_t diff = (seq_t)(currpos - prevpos);

            if (!first && diff == diff_prev)
                ++count;

            diff_prev = diff;
            prevpos = currpos;
            first = false;
        }
        tmp_n >>= 1;
        ++bitpos;
    }

    return count;
}
