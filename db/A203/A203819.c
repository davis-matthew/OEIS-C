/*
 * A203819  Number of (n+1)X2 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* compute 9^n in arbitrary precision (if needed) */
    seq_t pow = big_pow((seq_t)9, (seq_t)n);
    /* multiply by 16 */
    return mul_small(pow, 16);
}
