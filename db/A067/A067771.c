/*
 * A067771  Number of vertices in Sierpiński triangle of order n.
 * https://oeis.org/A067771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3*(3^n + 1)/2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 3^n using the provided pow3 helper */
    seq_t power = pow3(n);

    /* (3^n + 1) is even for all n >= 0, so divide first to keep
       intermediate values small (helps avoid overflow in 64‑bit builds). */
    seq_t half = (power + 1) / 2;

    /* Final result */
    return half * 3;
}
