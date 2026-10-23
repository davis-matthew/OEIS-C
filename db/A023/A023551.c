/*
 * A023551  Self-convolution of natural numbers >= 3.
 * https://oeis.org/A023551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 15*n + 38)/6
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 2560, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n * (seq_t)n;                 /* n^2 */
    seq_t term = nn + (seq_t)15 * (seq_t)n + (seq_t)38;  /* n^2 + 15*n + 38 */
    seq_t prod = (seq_t)n * term;                     /* n * term */
    return prod / (seq_t)6;                           /* divide by 6 */
}
