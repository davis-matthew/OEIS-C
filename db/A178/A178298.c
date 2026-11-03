/*
 * A178298  Number of collinear point triples in a 5 X 5 X 5 X...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n + 9^n + (13^n)/2 - (5^(n+1))/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow7  = ipow_idx(7, n);
    seq_t pow9  = ipow_idx(9, n);
    seq_t pow13 = ipow_idx(13, n);
    seq_t pow5  = ipow_idx(5, n + 1);

    seq_t half13 = pow13 / 2;
    seq_t half5  = pow5  / 2;

    return pow7 + pow9 + half13 - half5;
}
