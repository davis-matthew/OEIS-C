/*
 * A168299  a(n) = 1 + 3^n * n^3.
 * https://oeis.org/A168299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 3^n * n^3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p  = pow3(n);                  /* 3^n */
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;  /* n^3 */
    return (p * n3) + (seq_t)1;
}
