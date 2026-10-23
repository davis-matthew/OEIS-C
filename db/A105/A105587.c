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
 * Helpers:   mobius_uint64_local, squarefree_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_uint64_local.h"
#include "../utils/squarefree_upto.h"

/*------------------------------------------------------------------*/

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    int mu_n = mobius_uint64_local(n);
    if (mu_n != 0) {
        /* n is squarefree: a(n) = n + 1                               */
        return (seq_t)n + (seq_t)1;
    } else {
        /* n is nonsquarefree: a(n) = n - (#squarefree ≤ n)             */
        seq_t sq = squarefree_upto(n);
        return (seq_t)n - sq;
    }
}
