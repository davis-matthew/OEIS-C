/*
 * A171270  a(n) is the only number m such that m = pi(1^(1/n)) + pi(2^(1/n))
 *          + ... + pi(m^(1/n)).
 * https://oeis.org/A171270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 3^n + 2^n - 2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t term2 = pow2(n);   /* 2^n */
    seq_t term3 = pow3(n);   /* 3^n */
    return term3 + term2 - (seq_t)2;
}
