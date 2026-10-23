/*
 * A167779  Subsequence of A167708 whose indices are congruent to 4 mod 5,
 *          i.e., a(n) = A167708(5n+4).
 * https://oeis.org/A167779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 105, a(1) = 35634, a(n+2) = 340*a(n+1) - a(n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)105;
    if (n == 1) return (seq_t)35634;

    seq_t prev2 = (seq_t)105;   /* a(0) */
    seq_t prev1 = (seq_t)35634; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)340 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
