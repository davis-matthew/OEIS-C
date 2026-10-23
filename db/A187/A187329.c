/*
 * A187329  Floor((3-sqrt(5))n).
 * https://oeis.org/A187329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3-sqrt(5))*n)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = (seq_t)5 * nn * nn;      /* compute 5*n*n */
    seq_t s = floor_sqrt(prod);          /* floor(n*sqrt(5))  */
    /* a(n) = 3*n - ceil(n*sqrt(5)) = 3*n - (s + 1) for n>0 */
    return (seq_t)3 * nn - s - 1;
}
