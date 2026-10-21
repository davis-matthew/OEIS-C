/*
 * A061225  Numbers of the form k^3 + (k + 1)^4 + (k + 2)^5 + (k + 3)^6.
 * https://oeis.org/A061225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 + (n+1)^4 + (n+2)^5 + (n+3)^6
 * Range:     u8 none, u16 n <= 3, u32 n <= 37, u64 n <= 1622, u128 n <= 10000,
 *            big any n
 * Helpers:   pow_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_exact.h"

seq_t a(idx_t n)
{
    seq_t k   = (seq_t)n;
    seq_t kin1 = k + 1;
    seq_t kin2 = k + 2;
    seq_t kin3 = k + 3;

    seq_t term3 = pow_exact(k,   3);
    seq_t term4 = pow_exact(kin1, 4);
    seq_t term5 = pow_exact(kin2, 5);
    seq_t term6 = pow_exact(kin3, 6);

    return term3 + term4 + term5 + term6;
}
