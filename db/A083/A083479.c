/*
 * A083479  The natural numbers with all terms of A033638 inserted.
 * https://oeis.org/A083479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = (n+2) - ceil(sqrt(4*n)) for n > 0, a(0) = 0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N      = (seq_t)n;
    seq_t four_n = N * ((seq_t)4);

    seq_t s = floor_sqrt_seq(four_n);           /* floor sqrt of 4*n   */
    if (s * s < four_n)                         /* if not a perfect square, get the ceil  */
        s++;

    return N + 2 - s;                           /* a(n) = n + 2 - ceil(sqrt(4*n)) */
}
