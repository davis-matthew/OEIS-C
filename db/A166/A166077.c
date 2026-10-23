/*
 * A166077  a(n)=2^(n(3n-1)/2).
 * https://oeis.org/A166077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n*(3*n-1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nseq = n;
    seq_t exp = nseq * (nseq * 3 - 1) / 2;
    seq_t two = 2;
    return big_pow(two, exp);
}
