/*
 * A087629  Generalized Jacobsthal sequence.
 * https://oeis.org/A087629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = J(n+1) if n mod 3 ≠ 0 J(n+1) – J(n) if n mod 3 = 0 where
 *            J(m) = (2^m – (−1)^m) / 3 (Jacobi–type sequence, see A001045)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t jacob(idx_t m)
{
    seq_t two = pow2(m);           /* 2^m */
    if ((m & 1) == 0)             /* m even => (−1)^m = 1  */
        two -= (seq_t)1;          /* numerator = 2^m – 1   */
    else                          /* m odd  => (−1)^m = –1 */
        two += (seq_t)1;          /* numerator = 2^m + 1   */
    return two / (seq_t)3;
}

seq_t a(idx_t n)
{
    if ((n % 3) == 0)             /* n = 3k  */
        return jacob(n + 1) - jacob(n);
    else                          /* n = 3k+1 or 3k+2 */
        return jacob(n + 1);
}
