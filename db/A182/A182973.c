/*
 * A182973  Denominators of positive rationals < 1 arranged by increasing sum
 *          of numerator and denominator then by increasing numerator.
 * https://oeis.org/A182973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, nice, tabf
 * Formula:   a(n) is the denominator of the n-th reduced fraction a/b (0 < a
 *            < b) sorted by increasing sum a+b, then by increasing numerator
 *            a
 * Range:     u8 n <= 9973, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many denominators generated so far */
    for (idx_t sum = 3; ; ++sum) {          /* sum = a + b, minimal sum is 3 */
        for (idx_t a = 1; a * 2 < sum; ++a) { /* a < b  → a < sum/2 */
            idx_t b = sum - a;
            if (gcd((seq_t)a, (seq_t)b) == (seq_t)1) {
                ++count;
                if (count == n) return (seq_t)b;
            }
        }
    }
}
