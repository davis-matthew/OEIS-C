/*
 * A160939  a(n) = n + digital sum (n-1).
 * https://oeis.org/A160939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + digit_sum(n-1)
 * Range:     u8 n <= 245, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nval = (seq_t)n;
    seq_t nminus1 = (seq_t)(n - 1);
    return nval + digit_sum(nminus1);
}
