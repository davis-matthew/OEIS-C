/*
 * A048014  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t=1}^{min( floor((n-1)/3), floor((n -
 *            floor((n-1)/3))/5) )} * C( floor((n-1)/3), t ) * C( n -
 *            floor((n-1)/3), 5t ) * Note: C(n,k) is the binomial coefficient
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n <= 5 no subset can satisfy the condition. */
    if (n <= 5)
        return 0;

    idx_t small = (n - 1) / 3;          /* elements <= (n-1)/3                   */
    idx_t big   = n - small;            /* elements >  (n-1)/3                   */
    idx_t t_max = small;                /* t = number of small elements chosen   */

    idx_t lim = big / 5;                /* 5t <= big  =>  t <= big/5             */
    if (lim < t_max)
        t_max = lim;                    /* final bound for t                     */

    seq_t result = 0;
    for (idx_t t = 1; t <= t_max; ++t)
    {
        seq_t ways_small = binomial(small, t);
        seq_t ways_big   = binomial(big, 5 * t);
        result += ways_small * ways_big;
    }
    return result;
}
