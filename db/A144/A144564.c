/*
 * A144564  Bisection of A147757.
 * https://oeis.org/A144564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1)=1, a(2)=101, a(3)=10101; for n>=4: a(n)=91*(10^(2*n-3)-1)/9
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)101;
    if (n == 3) return (seq_t)10101;
    idx_t exp = 2*n - 3;
    seq_t val = pow10(exp);
    seq_t tmp = val - (seq_t)1;
    seq_t res = tmp * (seq_t)91;
    res = res / (seq_t)9;
    return res;
}
