/*
 * A160943  a(n) = n + digital sum(n-1) + digital sum(n+1).
 * https://oeis.org/A160943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + digit_sum(n-1) + digit_sum(n+1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for offset 1, handle gracefully */
    seq_t sn   = (seq_t)n;
    seq_t dsm  = digit_sum((seq_t)(n - 1));
    seq_t dsp  = digit_sum((seq_t)(n + 1));
    return sn + dsm + dsp;
}
