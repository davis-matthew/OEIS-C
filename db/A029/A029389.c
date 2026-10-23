/*
 * A029389  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of non‑negative integer solutions to
 * 5*a + 6*b + 9*c + 12*d = n   (i.e. partitions of n into parts 5,6,9,12)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate over d and c */
    for (idx_t d = 0; d <= n / 12; ++d) {
        idx_t rem1 = n - 12 * d;                 /* remaining after choosing d */
        for (idx_t c = 0; c <= rem1 / 9; ++c) {
            idx_t rem0 = rem1 - 9 * c;           /* remaining after choosing c */
            idx_t kmax = rem0 / 6;               /* maximum possible b */
            idx_t b0   = rem0 % 5;               /* first b ≡ rem0 (mod 5) */
            if (kmax >= b0) {
                idx_t num_b = (kmax - b0) / 5 + 1; /* number of admissible b */
                count += (seq_t)num_b;
            }
        }
    }
    return count;
}
