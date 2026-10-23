/*
 * A048061  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let L = ⌊(n-4)/2⌋. For each j ≥ 1 with j ≤ L and 2j ≤ n-L, count
 *            subsets with exactly j elements from {1,…,L} and 2j elements
 *            from {L+1,…,n}. The number of ways is C(L,j)·C(n-L,2j). Then
 *            a(n) = Σ_{j} C(L,j)·C(n-L,2j)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n < 6 gives no valid subset (size must be multiple of 3). */
    if (n < 6)
        return (seq_t)0;

    idx_t L = (n >= 4 ? (n - 4) / 2 : 0);   /* low part size */
    idx_t N = n - L;                         /* high part size */

    idx_t maxJ = L;
    if (N / 2 < maxJ)
        maxJ = N / 2;                        /* ensure 2j <= N */

    seq_t result = 0;

    for (idx_t j = 1; j <= maxJ; ++j)
    {
        seq_t low  = binom(L, j);            /* C(L, j)   */
        seq_t high = binom(N, 2 * j);        /* C(N, 2j)  */
        result += low * high;
    }

    return result;
}
