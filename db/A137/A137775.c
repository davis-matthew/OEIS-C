/*
 * A137775  Number of triples of permutations on n letters such that for each
 *          j, exactly one of the permutations fixes j and the other two have
 *          the same image on j.
 * https://oeis.org/A137775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * (a(n-1) + 3*a(n-2)), with a(0)=1, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t prevprev = (seq_t)1; /* a(0) */
    seq_t prev     = (seq_t)0; /* a(1) */

    /* compute iteratively */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t factor = (seq_t)(i - 1);          /* (i-1) */
        seq_t term   = prev + (seq_t)3 * prevprev;
        seq_t cur    = factor * term;

        prevprev = prev;
        prev     = cur;
    }

    return prev;
}
