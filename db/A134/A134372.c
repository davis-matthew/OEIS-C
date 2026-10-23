/*
 * A134372  a(n) = ((2n)!)^2.
 * https://oeis.org/A134372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n)!)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* Compute 2*n in the index type to avoid overflow in multiplication. */
    idx_t m = n << 1;               /* m = 2*n */
    seq_t f = factorial(m);         /* f = (2*n)! */
    return f * f;                   /* a(n) = ((2n)!)^2 */
}
