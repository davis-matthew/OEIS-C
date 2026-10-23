/*
 * A128794  a(n) = n^3*8^n.
 * https://oeis.org/A128794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * 8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t ns = (seq_t)n;
    seq_t n3 = ns * ns * ns;
    seq_t pow8 = seq_pow((seq_t)8, n);
    return n3 * pow8;
}
