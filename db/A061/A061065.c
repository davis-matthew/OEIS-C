/*
 * A061065  For n <= 6, entry of maximal modulus in the inverse of the n-th
 *          Hilbert matrix. For n >= 3, this is the (n-1,n-1)-th entry.
 * https://oeis.org/A061065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n-2)^2)*(2n-3)*C(2n-4, n-2)^2 for n >= 3; a(1) = 1,
 *            a(2) = 12
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)12;

    /* (2n-2)^2  */
    idx_t two_n_minus2 = 2 * n - 2;
    seq_t term1 = (seq_t)two_n_minus2 * (seq_t)two_n_minus2;

    /* 2n-3  */
    seq_t term2 = (seq_t)(2 * n - 3);

    /* C(2n-4, n-2)  */
    seq_t bin = binom(2 * n - 4, n - 2);

    /* a(n) = term1 * term2 * bin^2  */
    seq_t res = term1;
    res *= term2;
    res *= bin;
    res *= bin;

    return res;
}
