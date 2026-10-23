/*
 * A024105  a(n) = 9^n - n^4.
 * https://oeis.org/A024105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - n^4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 9^n */
    seq_t pow9 = seq_pow((seq_t)9, n);

    /* n^4, computed in seq_t to carry any overflows that fit in seq_t */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;   /* n^2 */
    seq_t n4   = n2 * n2;   /* n^4 */

    return pow9 - n4;
}
