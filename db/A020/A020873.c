/*
 * A020873  a(n) is number of cycles in Moebius ladder M_n.
 * https://oeis.org/A020873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n^2 - n + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);                 /* 2^n   */
    seq_t n_sq = (seq_t)n * (seq_t)n;    /* n^2   */
    val += n_sq;                         /* + n^2 */
    val -= (seq_t)n;                     /* - n   */
    val += 1;                            /* + 1   */
    return val;
}
