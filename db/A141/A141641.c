/*
 * A141641  Digital sum of n is an even composite.
 * https://oeis.org/A141641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = nth positive integer whose digit sum is an even composite
 *            number * (i.e., digit sum >= 4 and even)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 0;
    while (count < n) {
        cur++;
        seq_t s = digit_sum(cur);
        if (s >= 4 && (s & 1) == 0) {
            count++;
            if (count == n) {
                return cur;
            }
        }
    }
    return 0; /* unreachable */
}
