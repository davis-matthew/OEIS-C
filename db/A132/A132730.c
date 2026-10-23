/*
 * A132730  Row sums of triangle A132729.
 * https://oeis.org/A132730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 2^(n+1) - 3*n + 1 (OEIS A132730)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
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
    seq_t val = pow2(n + 1);          /* 2^(n+1) */
    val -= ((seq_t)3 * (seq_t)n);     /* subtract 3*n */
    val += (seq_t)1;                  /* add 1 */
    return val;
}
