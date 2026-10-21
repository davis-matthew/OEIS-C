/*
 * A072629  Parity of n*floor(log n).
 * https://oeis.org/A072629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n * floor(log(n))) mod 2
 * Range:     u8 n <= 9375, u16 n <= 9375, u32 n <= 9375, u64 n <= 9375,
 *            u128 n <= 9375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, and n < 1 is undefined */
    double ln = log((double)n);    /* natural logarithm */
    idx_t k   = (idx_t)floor(ln);  /* floor(log(n)) */
    return ((n & 1u) && (k & 1u)) ? (seq_t)1 : (seq_t)0;
}
