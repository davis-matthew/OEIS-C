/*
 * A035852  Coordination sequence for A_26 lattice.
 * https://oeis.org/A035852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..26} C(27,d)*C(n-1,d-1)*C(26-d+n,n) , with
 *            a(0)=1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t res = (seq_t)0;
    idx_t d;
    idx_t n_minus1 = n - 1;
    for (d = 1; d <= 26; ++d)
    {
        seq_t term = binom(27, d);
        term *= binom(n_minus1, d - 1);
        idx_t a2 = 26 - d + n;          /*  (26-d)+n   */
        term *= binom(a2, n);
        res += term;
    }
    return res;
}
