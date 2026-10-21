/*
 * A091479  Number of graphs with n nodes. Nodes and edges labeled each from
 *          their own label set.
 * https://oeis.org/A091479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n(n-1)/2} binom(n(n-1)/2, k) * k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 0: a(0) = a(1) = 1 */
    if (n <= 1) return (seq_t)1;

    /* M = number of possible edges = n*(n-1)/2 */
    idx_t M = n * (n - 1) / 2;

    seq_t term = (seq_t)1;          /* term for j = M : M! / M! = 1   */
    seq_t sum  = (seq_t)0;

    /* Iterate j from M down to 0: term_j = M! / j!  */
    for (idx_t j = M; ; --j)
    {
        sum += term;                /* add term for current j            */
        if (j == 0) break;          /* finished with j = 0               */
        term *= (seq_t)j;           /* term_{j-1} = term_j * j           */
    }

    return sum;
}
