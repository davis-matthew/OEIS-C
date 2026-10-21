/*
 * A085762  Least integer m such that between m and 2m there are n triangular
 *          numbers.
 * https://oeis.org/A085762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A085762: least m such that interval [m,2m] contains exactly n triangular numbers.
 * The algorithm uses the following observation:
 *   For a fixed number of triangulars n, let u be the smallest index of the
 *   first triangular number inside the interval.
 *   Then the interval bounds satisfy
 *         ceil(T_{u+n-1} / 2) <= m <= T_u
 *   The minimal m for a given u is therefore m = ceil(T_{u+n-1}/2).
 *   The first u that satisfies the inequality m <= T_u yields the minimal m
 *   for the requested n.
 *
 *   Thus we iterate over u = 1,2,... and test the condition
 *     ceil(T_{u+n-1}/2) <= T_u
 *   returning the corresponding m as soon as it holds.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* Not defined for n = 0, but return 0 for safety */

    /* iterate over possible starting indices u */
    for (idx_t u = 1; ; ++u)
    {
        /* triangular numbers of u and of u+n-1 */
        seq_t t_u   = ((seq_t)u * ((seq_t)u + 1)) / 2;
        seq_t end1  = (seq_t)u + n - 1;
        seq_t t_end = (end1 * (end1 + 1)) / 2;

        /* ceil(t_end / 2) = (t_end + 1) / 2 */
        seq_t m_candidate = (t_end + 1) / 2;

        /* if the lower bound on m does not exceed the upper bound, we have found the answer */
        if (m_candidate <= t_u)
            return m_candidate;
    }
}
