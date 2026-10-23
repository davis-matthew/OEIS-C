/*
 * A180294  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-4.
 * https://oeis.org/A180294
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrangements of n indistinguishable balls in n
 *            boxes with maximum balls in any box equal to n-4. Computation:
 *            a(n) = f(n, n-4) - f(n, n-5) where f(n, m) = Σ_{j=0}^{⌊n/(m+1)⌋}
 *            (-1)^j * C(n, j) * C(n - j*(m+1) + n - 1, n - 1)
 * Range:     u8 none, u16 n <= 16, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: binomial function from "../utils/binom.h" provides C(a,b) for
   0 ≤ b ≤ a. */

#include "../utils/binom.h"

static seq_t _f_leq(idx_t n, idx_t m)
{
    /* Count weak compositions of n into n parts with each part ≤ m. */
    idx_t max_j = n / (m + 1);           /* floor division, safe since m+1 ≥ 1 */
    seq_t sum_even = 0, sum_odd = 0;

    for (idx_t j = 0; j <= max_j; ++j) {
        /* Binomial(n, j) */
        seq_t comb_n_j = binom(n, j);

        /* Top of the second binomial: n - j*(m+1) + n - 1 */
        idx_t top = n - j * (m + 1) + n - 1;
        if (top < n - 1)             /* binom(top, n-1) = 0 for top < n-1   */
            break;

        seq_t comb_top = binom(top, n - 1);

        seq_t term = comb_n_j * comb_top;

        if ((j & 1) == 0)            /* even j: add   */
            sum_even += term;
        else                         /* odd j:  subtract */
            sum_odd += term;
    }
    return sum_even - sum_odd;        /* sum_even ≥ sum_odd for all n,m */
}

seq_t a(idx_t n)
{
    /* OEIS offset is 5; behavior for n < 5 does not matter. */
    if (n < 5)
        return 0;

    /* Max per box = n-4; count solutions with max ≤ n-4 */
    seq_t max_leq = _f_leq(n, n - 4);

    /* Count solutions with max ≤ n-5; subtract to get exact max = n-4 */
    seq_t min_leq = _f_leq(n, n - 5);

    return max_leq - min_leq;
}
