/*
 * A153894  a(n) = 5*2^n - 1.
 * https://oeis.org/A153894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n - 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t v = pow2(n);          /* v = 2^n  */
    v = v * (seq_t)5;           /* v = 5*2^n */
    v -= (seq_t)1;              /* v = 5*2^n - 1 */
    return v;
}
