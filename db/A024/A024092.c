/*
 * A024092  a(n) = 8^n - n^4.
 * https://oeis.org/A024092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - n^4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 8^n */
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);

    /* compute n^4 */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n4  = n2 * n2;

    /* result */
    return pow8 - n4;
}
