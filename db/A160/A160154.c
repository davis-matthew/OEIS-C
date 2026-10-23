/*
 * A160154  10^n-9n for n>=1.
 * https://oeis.org/A160154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - 9*n (for n >= 1); a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p = pow10(n);                  /* 10^n   */
    seq_t nine_n = (seq_t)9 * (seq_t)n;   /* 9*n    */
    return p - nine_n;
}
