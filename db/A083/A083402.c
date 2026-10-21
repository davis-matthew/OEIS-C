/*
 * A083402  Let A_n be the upper triangular matrix in the group GL(n,2) that
 *          has zero entries below the main diagonal and 1 elsewhere; a(n) is
 *          the size of the conjugacy class of this matrix in GL(n,2).
 * https://oeis.org/A083402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{(n-1)(n-2)/2} * ∏_{j=1}^{n} (2^j - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;          /* product part */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = ((seq_t)1 << j) - 1;      /* 2^j - 1 */
        res *= term;
    }
    if (n >= 2) {
        idx_t exp = (n - 1) * (n - 2) / 2;     /* exponent of the power of 2 */
        res <<= exp;                           /* multiply by 2^exp */
    }
    return res;
}
