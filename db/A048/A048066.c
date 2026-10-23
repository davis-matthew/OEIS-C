/*
 * A048066  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r≥1, 5r≤n, 2r≤⌊(n-4)/2⌋} C(⌊(n-4)/2⌋, 2r) *
 *            C(n-⌊(n-4)/2⌋, 3r)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    // For n < 5 the condition cannot be satisfied.
    if (n < 5)
        return 0;

    idx_t t = (n - 4) / 2;             // floor((n-4)/2)
    idx_t r_max = n / 5;
    if (t / 2 < r_max)
        r_max = t / 2;                 // min(n/5, t/2)

    if (r_max == 0)
        return 0;

    seq_t result = 0;
    for (idx_t r = 1; r <= r_max; ++r)
    {
        seq_t term = binom(t, 2 * r) * binom(n - t, 3 * r);
        result += term;
    }
    return result;
}
