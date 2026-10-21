/*
 * A093149  a(1) = 4; a(n) = (n^(n+1)+2*n-3)/(n-1) for n > 1.
 * https://oeis.org/A093149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^(n+1) + 2*n - 3) / (n-1), a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   mpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mpow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 4;
    seq_t s = (seq_t)n;
    seq_t power = mpow(s, n + 1);           /* n^(n+1)  */
    seq_t two_n = s + s;                    /* 2*n      */
    seq_t numerator = power + two_n - 3;    /* n^(n+1)+2*n-3 */
    seq_t denominator = n - 1;
    return numerator / denominator;
}
