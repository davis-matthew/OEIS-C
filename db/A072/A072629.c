/*
 * A072629  Parity of n*floor(log n).
 * https://oeis.org/A072629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n * floor(log(n))) mod 2
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9375 (2026-11-01)
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
