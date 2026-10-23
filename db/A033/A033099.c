/*
 * A033099  Number of 3's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{b=2}^{n+1} number of digits equal to 3 in the base‑b
 *            representation of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n + 1;
    for (idx_t b = 2; b <= limit; ++b) {
        idx_t temp = n;
        while (temp) {
            if ((temp % b) == (idx_t)3)
                ++sum;
            temp /= b;
        }
    }
    return sum;
}
