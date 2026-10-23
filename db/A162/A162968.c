/*
 * A162968  Number of pairs of consecutive non-fixed points in all
 *          permutations of {1,2,...,n}.
 * https://oeis.org/A162968
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * (n^2 - 3*n + 3) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for n < 2 */

    /* compute (n-1)!  */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact = fact * (seq_t)i;

    /* compute (n^2 - 3*n + 3)  */
    seq_t nseq = (seq_t)n;
    seq_t term = nseq * nseq - (seq_t)3 * nseq + (seq_t)3;

    return fact * term;
}
