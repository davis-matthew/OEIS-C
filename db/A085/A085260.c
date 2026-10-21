/*
 * A085260  Ratio-determined insertion sequence I(0.0833344) (see the link
 *          below).
 * https://oeis.org/A085260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 12, a(n) = 13*a(n-1) - a(n-2) for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed elsewhere */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;

    seq_t prev = (seq_t)1;   /* a(1) */
    seq_t curr = (seq_t)12;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = (seq_t)13 * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
