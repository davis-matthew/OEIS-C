/*
 * A181565  a(n) = 3*2^n + 1.
 * https://oeis.org/A181565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);          /* compute 2^n */
    val = val * (seq_t)3;         /* multiply by 3 */
    return val + (seq_t)1;        /* add 1 */
}
