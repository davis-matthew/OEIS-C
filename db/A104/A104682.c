/*
 * A104682  a(n) = Sum_{j=0..14} n^j.
 * https://oeis.org/A104682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^14 + n^13 + n^12 + n^11 + n^10 + n^9 + n^8 + n^7 + n^6
 *            + n^5 + n^4 + n^3 + n^2 + n + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;   /* n^0 */
    seq_t term   = 1;   /* current n^j */
    seq_t nn     = (seq_t)n;

    for (idx_t i = 1; i <= 14; ++i) {
        term *= nn;   /* term = n^i */
        result += term;
    }
    return result;
}
