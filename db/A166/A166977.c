/*
 * A166977  Jacobsthal-Lucas numbers A014551, except a(0) = 0.
 * https://oeis.org/A166977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 2^n + (-1)^n for n > 0, a(0) = 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p = pow2(n);          /* p = 2^n */
    if (n & 1)                  /* n is odd  -> (-1)^n = -1 */
        return p - (seq_t)1;    /* a(n) = 2^n - 1 */
    else                        /* n is even -> (-1)^n = +1 */
        return p + (seq_t)1;    /* a(n) = 2^n + 1 */
}
