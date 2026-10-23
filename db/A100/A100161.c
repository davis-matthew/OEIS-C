/*
 * A100161  Structured disdyakis dodecahedral numbers (vertex structure 9).
 * https://oeis.org/A100161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (40*n^3 - 48*n^2 + 14*n)/6
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 487, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */
    seq_t num = ((seq_t)40 * n3) - ((seq_t)48 * n2) + ((seq_t)14 * nn);
    return num / (seq_t)6;
}
