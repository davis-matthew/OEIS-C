/*
 * A048076  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{min( L , ⌊(n−L)/4⌋ )} C(L , t) · C(n−L , 4·t)
 *            where L = ⌊(n+2)/3⌋. This counts the non‑empty subsets of
 *            {1,…,n} in which exactly 1/5 of the elements are ≤ (n+2)/3
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute L = floor((n+2)/3) */
    idx_t L = (n + 2) / 3;
    /* t_max is the maximum t with t ≤ L and 4t ≤ n−L */
    idx_t t_max1 = (n - L) / 4;
    idx_t t_max  = (t_max1 < L) ? t_max1 : L;

    seq_t result = 0;
    for (idx_t t = 1; t <= t_max; ++t) {
        seq_t c1 = binom(L, t);                       /* C(L, t)   */
        seq_t c2 = binom(n - L, 4 * t);               /* C(n−L, 4t)*/
        result += c1 * c2;
    }
    return result;
}
