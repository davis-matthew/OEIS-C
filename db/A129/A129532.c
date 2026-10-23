/*
 * A129532  a(n) = 3*n*(n-1)*4^(n-2).
 * https://oeis.org/A129532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n-1)*4^(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) and a(1) are 0 */
    if (n < 2)
        return 0;

    seq_t sn = n;
    seq_t part = ((seq_t)3) * sn * (sn - (seq_t)1);
    seq_t pow4 = big_pow((seq_t)4, sn - (seq_t)2);
    return part * pow4;
}
