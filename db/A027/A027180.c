/*
 * A027180  a(n) = Sum_{0<=j<=i<=n} A027170(i, j).
 * https://oeis.org/A027180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*2^n - 2*n^2 - 10*n - 17
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 18 * pow2(n);
    val -= ((2 * nn * nn) + (10 * nn) + (seq_t)17);
    return val;
}
