/*
 * A085385  Binomial transform of hexagonal pyramidal numbers A002412.
 * https://oeis.org/A085385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*2^n(2n^2 + 9n + 1)/4!
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;               /* Convert index to seq_t */
    seq_t nsq = sn * sn;               /* n^2 */
    seq_t poly = 2 * nsq + 9 * sn + (seq_t)1;
    seq_t num = sn * poly;             /* n * (2n^2 + 9n + 1) */
    seq_t pow = pow2(n);               /* 2^n */
    num = num * pow;                   /* Multiply by 2^n */
    num = num / (seq_t)24;             /* Divide by 4! = 24 */
    return num;
}
