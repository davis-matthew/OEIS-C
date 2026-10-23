/*
 * A105423  Number of compositions of n+2 having exactly two parts equal to 1.
 * https://oeis.org/A105423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 39, u64 n <= 82, u128 n <= 172,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code for OEIS A105423 – Number of compositions of n+2 having exactly two parts equal to 1.
   Formula used: a(0) = 1, for n ≥ 1:
   a(n) = Σ_{m=1}^{⌊n/2⌋} C(n-m-1, m-1) * C(m+2, 2)   */
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t sum = (seq_t)0;
    idx_t max_m = n / 2;
    for (idx_t m = 1; m <= max_m; ++m) {
        seq_t t1 = binom(n - m - 1, m - 1); /* compositions of n-m into m parts each ≥1 */
        seq_t t2 = binom(m + 2, 2);         /* choose positions of the two 1’s among m+2 parts */
        sum += t1 * t2;
    }
    return sum;
}
