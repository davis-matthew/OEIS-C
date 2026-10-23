/*
 * A187991  Number of nondecreasing arrangements of 7 numbers x(i) in
 *          -(n+5)..(n+5) with the sum of sign(x(i))*2^|x(i)| zero.
 * https://oeis.org/A187991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^3 + 47*n^2 + 287*n + 486) / 2
 * Range:     u8 none, u16 n <= 24, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS sequence A187991 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 2 * nn * nn * nn
              + 47 * nn * nn
              + 287 * nn
              + 486;
    return val / 2;
}
