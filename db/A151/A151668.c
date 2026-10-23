/*
 * A151668  G.f.: Product_{k>=0} (1 + 2*x^(3^k)).
 * https://oeis.org/A151668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if the ternary expansion of n contains a digit 2;
 *            otherwise a(n) = 2^{(# of 1's in the ternary expansion of n)}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t cnt = 0;

    while (m > 0) {
        idx_t d = m % 3;
        if (d == 2)
            return (seq_t)0;
        if (d == 1)
            ++cnt;
        m /= 3;
    }

    return pow2(cnt);
}
