/*
 * A048021  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of nonempty subsets of {1..n} in which exactly 1/5
 *            of the elements are <= floor((n-2)/3). This equals the sum over
 *            t≥1 of C(m, t) * C(n-m, 4t), where m = floor((n-2)/3)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but guard */

    idx_t m = (n >= 2) ? (n - 2) / 3 : 0;
    idx_t high = n - m;

    seq_t total = 0;

    for (idx_t t = 1; ; ++t)
    {
        /* stop if any size constraint violated */
        if (5 * t > n) break;
        if (t > m) break;
        if (4 * t > high) break;

        seq_t low_choose  = binomial(m, t);
        seq_t high_choose = binomial(high, 4 * t);

        total += low_choose * high_choose;
    }

    return total;
}
