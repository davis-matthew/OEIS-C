/*
 * A205248  Number of (n+1) X 2 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock the same.
 * https://oeis.org/A205248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * (3^n + 1) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t v = pow3(n);   /* v = 3^n */
    v += 1;              /* v = 3^n + 1 */
    v *= 4;              /* v = 4 * (3^n + 1) */
    return v;
}
