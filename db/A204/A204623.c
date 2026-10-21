/*
 * A204623  Number of (n+1)X2 0..2 arrays with every 2X2 subblock having
 *          unequal diagonal elements or unequal antidiagonal elements, and
 *          new values 0..2 introduced in row major order.
 * https://oeis.org/A204623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 * 8^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    seq_t pow8 = big_pow((seq_t)8, (seq_t)(n - 1));
    return pow8 * ((seq_t)12);
}
