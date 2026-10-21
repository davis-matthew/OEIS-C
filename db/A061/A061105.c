/*
 * A061105  Smallest number whose sum of digits is n^3.
 * https://oeis.org/A061105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((n mod 3)^3+1)*10^floor[n^3/9]-1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* leading digit: (n mod 3)^3 + 1, values 1, 2, or 9 */
    idx_t m = n % 3;
    seq_t m_seq = (seq_t)m;
    seq_t lead = m_seq * m_seq * m_seq + 1;

    /* exponent e = floor(n^3 / 9) */
    idx_t e = ((idx_t)n * n * n) / 9;

    /* result = lead * 10^e - 1 */
    seq_t result = lead * pow10(e);
    return result - 1;
}
