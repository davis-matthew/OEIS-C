/*
 * A077611  Number of adjacent pairs of form (odd,odd) among all permutations
 *          of {1,2,...,n}.
 * https://oeis.org/A077611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/2) * (ceil(n/2)-1) * (n-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1 and 2 there are no adjacent odd–odd pairs. */
    if (n < 3) return (seq_t)0;

    /* number of odd integers in 1..n */
    seq_t odd_count = (seq_t)(n + 1) / 2;

    /* number of ordered odd–odd pairs (r,s) with r≠s */
    seq_t pair_cnt = odd_count * (odd_count - 1);

    /* (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) {
        fact *= (seq_t)i;
    }

    return pair_cnt * fact;
}
