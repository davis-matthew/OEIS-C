/*
 * A048041  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{t=1}^{min(a,⌊b/3⌋)} C(a, t)·C(b, 3t), where a =
 *            ⌊(n+1)/3⌋ and b = n – a
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A048041: Number of nonempty subsets of {1,…,n} in which
   exactly 1/4 of the elements are ≤ (n+1)/3.  */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Small elements  = {1,…,m} */
    idx_t m = (n + 1) / 3;
    idx_t a_small = m;          /* |A| */
    idx_t b_big   = n - m;      /* |B| */

    /* t ranges such that t ≤ a_small and 3t ≤ b_big, t ≥ 1 */
    idx_t max_t = a_small;
    idx_t t3 = b_big / 3;
    if (t3 < max_t) max_t = t3;

    seq_t total = 0;

    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t term = binomial(a_small, t) * binomial(b_big, 3 * t);
        total += term;
    }

    return total;
}
