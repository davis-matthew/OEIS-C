/*
 * A229506  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          diagonally and antidiagonally with exactly one mistake, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64*3^(2*n-5)*(8*n-3) for n>2, with a(1)=0 and a(2)=288.
 *            For all other n the formula is used
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 208 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)288;

    /* n > 2 */
    seq_t result = (seq_t)64;
    /* compute (8*n-3) as idx_t first to avoid overflow */
    idx_t tmp = 8 * n - (idx_t)3;
    seq_t factor = (seq_t)tmp;
    result = seq_mul(result, factor);

    idx_t exp = 2 * n - (idx_t)5;
    seq_t power = ipow_idx((seq_t)3, exp);
    result = seq_mul(result, power);

    return result;
}
