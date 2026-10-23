/*
 * A036828  A036827/2.
 * https://oeis.org/A036828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n^3-3*n^2+9*n-13)+13
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* poly = n^3 - 3*n^2 + 9*n - 13 */
    seq_t poly = sn * sn * sn;          /* n^3 */
    poly -= (seq_t)3 * sn * sn;         /* -3*n^2 */
    poly += (seq_t)9 * sn;              /* +9*n  */
    poly -= (seq_t)13;                  /* -13   */
    seq_t res = pow2(n) * poly;         /* 2^n * poly */
    res += (seq_t)13;                   /* +13 */
    return res;
}
