/*
 * A229473  Number of defective 4-colorings of an n X 2 0..3 array connected
 *          horizontally, antidiagonally and vertically with exactly one
 *          mistake, and colors introduced in row-major 0..3 order.
 * https://oeis.org/A229473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-5)*(10*n-9) for n>2; a(1)=1, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    /* for n >= 3 */
    idx_t exp = 2*n - 5;
    seq_t pow_val = pow2(exp);
    seq_t factor  = (seq_t)(10*n - 9);
    return pow_val * factor;
}
