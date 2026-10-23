/*
 * A033095  Number of 1's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{b=2}^{n+1} (#ones in the representation of n in base
 *            b)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
 
seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* loop over bases 2 .. n+1 */
    for (idx_t b = 2; b <= n + 1; ++b) {
        idx_t t = n;
        while (t != 0) {
            if (t % b == 1) {
                ++sum;
            }
            t /= b;
        }
    }
    return sum;
}
