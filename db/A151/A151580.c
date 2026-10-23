/*
 * A151580  Number of permutations of 1..n arranged in a circle with exactly 7
 *          adjacent element pairs in decreasing order.
 * https://oeis.org/A151580
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Eulerian(n-1, 6) (Eulerian number A(n-1,6), OEIS
 *            A001243)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Assume seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* For n < 7 the value is 0 by definition. */
    if (n < 7)
        return (seq_t)0;

    idx_t m = n - 1;      /* compute Eulerian(m, 6) */

    /* cur[k] holds A(current_i, k) for current i, 0 ≤ k ≤ 6 */
    seq_t cur[7] = {0};
    cur[0] = 1;           /* A(1,0) = 1 */

    for (idx_t i = 2; i <= m; ++i)
    {
        seq_t next[7] = {0};

        for (size_t k = 0; k <= 6; ++k)
        {
            /* (k+1)*A(i-1, k) */
            seq_t term = (seq_t)(k + 1) * cur[k];

            /* (i-k)*A(i-1, k-1) if k>0 */
            if (k > 0)
            {
                idx_t i_minus_k = i - (idx_t)k;
                term += (seq_t)i_minus_k * cur[k - 1];
            }

            next[k] = term;
        }

        /* copy next row into cur for the next iteration */
        for (size_t k = 0; k <= 6; ++k)
            cur[k] = next[k];
    }

    /* cur[6] is A(m, 6). Multiply by n to get a(n). */
    return (seq_t)n * cur[6];
}
