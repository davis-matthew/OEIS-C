/*
 * A120800  a(n) = 3^(n^2) + 2^(n^2).
 * https://oeis.org/A120800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n^2) + 2^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t exp = nseq * nseq;
    seq_t p3 = big_pow((seq_t)3, exp);
    seq_t p2 = big_pow((seq_t)2, exp);
    return p3 + p2;
}
