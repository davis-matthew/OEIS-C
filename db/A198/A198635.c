/*
 * A198635  Total number of round trips, each of length 2*n on the graph P_5
 *          (o-o-o-o-o).
 * https://oeis.org/A198635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5; a(n) = 2*3^n + 2 for n > 0. (P_5 round‑trip walks)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;
    seq_t p = pow3(n);      /* 3^n */
    return p * 2 + 2;       /* 2*3^n + 2 */
}
