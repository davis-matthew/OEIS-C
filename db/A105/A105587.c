/*
 * A105587  Row sums of a Möbius-binomial triangle.
 * https://oeis.org/A105587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+1 if μ(n)≠0 (squarefree), otherwise a(n) = number of
 *            nonsquarefree integers k with 1 ≤ k ≤ n. For n = 0, a(0) = 1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 74 (2026-11-01)
 * Helpers:   mobius_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_u64.h"

/* Count of squarefree integers u with 1 ≤ u ≤ n
   (uses the classic summation over μ(d)).          */
static seq_t squarefree_upto(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n) ++limit;   /* integer sqrt */

    for (idx_t d = 1; d <= limit; ++d) {
        int mu = mobius_u64(d);
        if (mu == 0) continue;
        idx_t term = n / (d * d);                    /* floor division */
        if (mu == 1)
            sum += (seq_t)term;
        else
            sum -= (seq_t)term;   /* mu == -1 */
    }
    return sum;
}

/*------------------------------------------------------------------*/

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    int mu_n = mobius_u64(n);
    if (mu_n != 0) {
        /* n is squarefree: a(n) = n + 1                               */
        return (seq_t)n + (seq_t)1;
    } else {
        /* n is nonsquarefree: a(n) = n - (#squarefree ≤ n)             */
        seq_t sq = squarefree_upto(n);
        return (seq_t)n - sq;
    }
}
