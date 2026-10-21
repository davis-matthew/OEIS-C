/*
 * A213937  Row sums a(n) of triangle A213936: number of representative
 *          necklaces with n beads (C_N symmetry) corresponding to all color
 *          signatures given by the partitions [1^n], [2,1^(n-2)], ...,
 *          [n-1,1], [n].
 * https://oeis.org/A213937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002627(n-1) + 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* out of bounds, harmless */

    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t term = 1;          /* term = (n-1)! / k! for current k */
    seq_t sum_terms = 1;    /* k = n-1 contributes 1 */

    for (idx_t i = n - 2; i >= 1; --i) {
        term *= (seq_t)(i + 1);   /* term = term * (i+1)  -> next k */
        sum_terms += term;        /* accumulate this term */
    }

    return sum_terms + (seq_t)1; /* final +1 per definition */
}
