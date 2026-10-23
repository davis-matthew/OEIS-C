/*
 * A141742  Starting from the 1 in the first line of triangle A141728 choose
 *          one of the three digits below it. Repeat down to the other rows.
 *          Sequence gives the numbers in base 10 expressed by the collected
 *          digits that cannot be reached following any possible path.
 * https://oeis.org/A141742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A141742: Missings in the path‑digit triangle A141728.
 * The missing numbers are grouped in intervals:
 *
 *  group i (i = 1,2,3,...)
 *      start S_i = 3 * 2^(i-1)
 *      length L_i = 1 + i(i-1)/2
 *      missing numbers: [S_i, S_i+L_i-1]
 *
 * The cumulative count up to group i is
 *      C_i = i + i(i+1)(i-1)/6
 *
 * For a given index n (0‑based) find the minimal i with C_i > n,
 * then offset = n - (C_i - L_i) and the value is S_i + offset.
 *
 * This function implements that formula.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cum = 0;           /* cumulative count up to current group   */
    seq_t start = 3;         /* S_i for the current group i           */
    idx_t  i = 1;            /* group index (1‑based)                  */

    while (1)
    {
        /* length of group i: 1 + i(i-1)/2 */
        seq_t Li = 1 + (seq_t)i * (i - 1) / 2;
        cum += Li;

        /* check if n lies in this group */
        if (cum > (seq_t)n)
        {
            seq_t offset = (seq_t)n - (cum - Li);
            return start + offset;
        }

        /* advance to next group: start doubles, i increments */
        start <<= 1;   /* multiply by 2 */
        ++i;
    }
}
