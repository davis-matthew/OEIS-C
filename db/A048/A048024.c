/*
 * A048024  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{t≥1, 4t≤⌊(n−2)/3⌋, t≤n−⌊(n−2)/3⌋} * binom(⌊(n−2)/3⌋,
 *            4t) · binom(n−⌊(n−2)/3⌋, t) * * The sum counts all nonempty
 *            subsets of {1,…,n} that contain exactly * 4/5 of their elements
 *            in the first ⌊(n−2)/3⌋ positions
 * Range:     u8 n <= 22, u16 n <= 37, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute m = floor((n-2)/3) safely for unsigned types. */
    idx_t m = (n <= 2) ? 0 : (n - 2) / 3;
    idx_t n_m = n - m;

    /* t must satisfy 4t ≤ m and t ≤ n_m.  The upper bound for t is
       min(m/4, n_m).  t must be at least 1. */
    idx_t t_max = m / 4;
    if (t_max > n_m)
        t_max = n_m;

    seq_t sum = 0;

    for (idx_t t = 1; t <= t_max; ++t)
    {
        seq_t c1 = binom(m, 4 * t);  /* C(m,4t) */
        seq_t c2 = binom(n_m, t);    /* C(n−m, t) */
        sum += c1 * c2;
    }

    return sum;
}
