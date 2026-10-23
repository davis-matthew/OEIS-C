/*
 * A123522  Not of the form n + [log_10 n].
 * https://oeis.org/A123522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n + n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t val = pow10(n);      /* 10^n  */
    val += (seq_t)n;           /* + n  */
    val -= 1;                  /* - 1  */
    return val;
}
