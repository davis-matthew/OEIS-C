/*
 * A036487  a(n) = floor((n^3)/2).
 * https://oeis.org/A036487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3)/2)
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 2017, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent forms: ((-1)^n - 1 + 2*n^3)/4, or n*(n^2) >> 1 for unsigned types */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* cast to the sequence value type first */
    seq_t res = nn * nn * nn;     /* compute n^3 safely within seq_t */
    res /= 2;                     /* floor division by 2 */
    return res;
}
