/*
 * A048012  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1, 3t≤⌊(n−1)/3⌋, 2t≤n−⌊(n−1)/3⌋} binomial(⌊(n−1)/3⌋,
 *            3t) · binomial(n−⌊(n−1)/3⌋, 2t)
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count non‑empty subsets of {1,…,n} in which exactly 3/5 of the
   elements are ≤ (n−1)/3. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t L = (n - 1) / 3;          /* first part contains numbers ≤ (n-1)/3 */
    idx_t R = n - L;                /* remaining numbers > (n-1)/3      */
    idx_t max_t = L / 3;            /* from 3t ≤ L                         */
    if (R / 2 < max_t)              /* from 2t ≤ R                         */
        max_t = R / 2;

    seq_t sum = 0;
    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t c1 = binomial(L, 3 * t);
        seq_t c2 = binomial(R, 2 * t);
        sum += c1 * c2;
    }
    return sum;
}
