/*
 * A171687  a(n) = (10^n)^4+(10^n+1)^4.
 * https://oeis.org/A171687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n)^4 + (10^n + 1)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* pow10(n) yields 10^n as seq_t. */
    seq_t t = pow10(n);

    /* (10^n)^4 */
    seq_t t4 = seq_pow(t, 4);

    /* (10^n + 1)^4 */
    seq_t t1plus1 = t + 1;
    seq_t u4 = seq_pow(t1plus1, 4);

    /* a(n) = t4 + u4 */
    return t4 + u4;
}
