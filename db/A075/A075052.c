/*
 * A075052  Write down squares and cubes in order, as in A002760. The sequence
 *          gives the first differences between terms.
 * https://oeis.org/A075052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002760(n+1) - A002760(n) (sorted squares and cubes
 *            without duplicates)
 * Range:     u8 n <= 18, u16 n <= 722, u32 n <= 9465, u64 n <= 9465,
 *            u128 n <= 9465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* i : index of the current square (i^2)
       j : index of the current cube   (j^3) */
    idx_t i = 0, j = 0;
    seq_t val = 0;
    seq_t term_n   = 0;   /* value at position n   */
    seq_t term_np1 = 0;   /* value at position n+1 */

    for (idx_t pos = 1; pos <= n + 1; ++pos)
    {
        seq_t sq = (seq_t)i * (seq_t)i;
        seq_t cu = (seq_t)j * (seq_t)j * (seq_t)j;

        /* Choose the next smallest unique value */
        if (sq == cu) {           /* common sixth power – take once */
            val = sq;
            ++i;
            ++j;
        } else if (sq < cu) {
            val = sq;
            ++i;
        } else {
            val = cu;
            ++j;
        }

        if (pos == n)   term_n   = val;
        if (pos == n+1) term_np1 = val;
    }

    return term_np1 - term_n;
}
