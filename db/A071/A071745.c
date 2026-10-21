/*
 * A071745  Expansion of (1+x^4*C)*C^4, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_{4,n} + (n>=4 ? F_{5,n-4} : 0) where F_{k,n} = k/(2n+k)
 *            * binom(2n+k , n) (Fuss‑Catalan number)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* F_{4,n} */
    idx_t denom4 = 2 * n + 4;
    seq_t bin4 = binom(denom4, n);
    seq_t f4 = ((seq_t)4 * bin4) / denom4;

    seq_t result = f4;

    /* Add F_{5,n-4} if n>=4 */
    if (n >= 4) {
        idx_t m = n - 4;
        idx_t denom5 = 2 * m + 5;
        seq_t bin5 = binom(denom5, m);
        seq_t f5 = ((seq_t)5 * bin5) / denom5;
        result += f5;
    }

    return result;
}
