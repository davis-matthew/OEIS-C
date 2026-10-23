/*
 * A132045  Row sums of triangle A132044.
 * https://oeis.org/A132045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n + 1 for n>=1, a(0)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t val = pow2(n);      /* compute 2^n */
    val -= (seq_t)n;          /* subtract n */
    val += 1;                 /* add 1 */
    return val;
}
