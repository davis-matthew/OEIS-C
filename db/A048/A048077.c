/*
 * A048077  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1} C(⌊(n+2)/3⌋, 2m) · C(n − ⌊(n+2)/3⌋, 3m) where m
 *            satisfies 5m ≤ n, 2m ≤ ⌊(n+2)/3⌋ and 3m ≤ n − ⌊(n+2)/3⌋
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Lower part size */
    idx_t M = (n + 2) / 3;
    /* Upper part size */
    idx_t U = n - M;

    /* Upper bound on m from the three inequalities */
    idx_t max_m = n / 5;
    if (M / 2 < max_m) max_m = M / 2;
    if (U / 3 < max_m) max_m = U / 3;

    if (max_m == 0) return 0;

    seq_t sum = 0;
    for (idx_t m = 1; m <= max_m; ++m)
    {
        seq_t c1 = binom(M, 2 * m);
        seq_t c2 = binom(U, 3 * m);
        sum += c1 * c2;
    }
    return sum;
}
