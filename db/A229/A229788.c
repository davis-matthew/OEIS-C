/*
 * A229788  a(n) = 6*2^n - n^2 - 5*n - 6.
 * https://oeis.org/A229788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - n^2 - 5*n - 6
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = pow2(n);              /* 2^n */
    res = res * (seq_t)6;             /* 6*2^n */
    seq_t ns = (seq_t)n;              /* n as seq_t */
    res = res - ns * ns;              /* subtract n^2 */
    res = res - ns * (seq_t)5;        /* subtract 5*n */
    res = res - (seq_t)6;             /* subtract 6 */
    return res;
}
