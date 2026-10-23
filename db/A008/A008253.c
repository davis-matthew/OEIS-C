/*
 * A008253  Coordination sequence for diamond.
 * https://oeis.org/A008253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(2m) = (5*(2m)^2 + 4)/2 = (5*n^2+4)/2, a(2m+1) =
 *            (5*(2m+1)^2 + 3)/2 = (5*n^2+3)/2 for n>0
 * Range:     u8 n <= 7, u16 n <= 160, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The build supplies the definitions of seq_t and idx_t.
   They are unsigned integer types of varying size.
   Arithmetic is performed in seq_t to avoid overflow between
   conversions of idx_t to seq_t. */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n^2 */
    seq_t num;

    if ((n & 1) == 0) {          /* even n */
        num = (seq_t)5 * sq + (seq_t)4;
    } else {                     /* odd n */
        num = (seq_t)5 * sq + (seq_t)3;
    }

    return num / (seq_t)2;       /* division by 2 is exact */
}
