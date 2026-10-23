/*
 * A017188  a(n) = (9*n + 2)^4.
 * https://oeis.org/A017188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 28, u64 n <= 7281,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute t = 9*n + 2 in seq_t to avoid overflow in intermediate types */
    seq_t t = (seq_t)9 * (seq_t)n + (seq_t)2;
    /* Compute t^4 = (t^2)^2 */
    seq_t t2 = t * t;
    return t2 * t2;
}
