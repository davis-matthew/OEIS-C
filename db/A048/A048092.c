/*
 * A048092  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{⌊n/6⌋} [5m ≤ ⌊(n+3)/3⌋ ∧ m ≤ n-⌊(n+3)/3⌋] ·
 *            binomial(⌊(n+3)/3⌋, 5m) · binomial(n-⌊(n+3)/3⌋, m)
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Early exit for tiny n */
    if (n < 6)
        return 0;

    /* L = floor((n+3)/3), H = n - L */
    idx_t L = (n + 3) / 3;
    idx_t H = n - L;

    seq_t total = 0;
    /* m ≥ 1, 6m ≤ n  →  m ≤ n/6 */
    idx_t maxm = n / 6;
    for (idx_t m = 1; m <= maxm; ++m) {
        if (5 * m <= L && m <= H) {
            seq_t low  = binomial(L, 5 * m);
            seq_t high = binomial(H, m);
            total += low * high;
        }
    }
    return total;
}
