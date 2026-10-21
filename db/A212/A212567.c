/*
 * A212567  Number of (w,x,y,z) with all terms in {1,...,n} and 2w+2x=3y+3z.
 * https://oeis.org/A212567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{⌊2n/3⌋} count_pairs(3k,n) * count_pairs(2k,n)
 *            where count_pairs(s,n) counts ordered pairs (x,y) with 1 ≤ x,y ≤
 *            n and x + y = s. For a fixed sum s, count_pairs(s,n) = (s ≤ n+1)
 *            ? (s-1) : (2*n - s + 1)
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   count_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_pairs.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t res = 0;
    idx_t maxk = (idx_t)((2 * n) / 3);   /* floor(2n/3) */

    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t sum_wx = 3 * k;   /* w + x */
        idx_t sum_yz = 2 * k;   /* y + z */

        seq_t f = count_pairs(sum_wx, n);
        seq_t g = count_pairs(sum_yz, n);

        res += f * g;
    }
    return res;
}
