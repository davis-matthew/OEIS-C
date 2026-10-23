/*
 * A048090  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{min(⌊L/4⌋, H)} binomial(L,4t)·binomial(H,t),
 *            where L = ⌊(n+3)/3⌋ and H = n – L. This counts the nonempty
 *            subsets of {1,…,n} in which exactly 4/5 of the elements are ≤ L
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t L = (n + 3) / 3;         /* floor((n+3)/3) */
    idx_t H = n - L;               /* elements > L */
    idx_t max_t = L / 4;           /* floor(L/4) */
    if (max_t > H) max_t = H;      /* cannot exceed H */

    seq_t result = 0;
    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t low  = binom(L, 4 * t);
        seq_t high = binom(H, t);
        result += low * high;
    }
    return result;
}
