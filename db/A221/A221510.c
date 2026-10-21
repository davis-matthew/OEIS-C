/*
 * A221510  Number of 0..3 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 3*a(n-3) + a(n-4) initial values:
 *            a(1)=0, a(2)=2, a(3)=3, a(4)=12
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)12;

    /* Compute the value iteratively; the recurrence is linear and
       of order four, so we keep the most recent four terms. */
    seq_t a1 = (seq_t)0;   /* a(n-4) */
    seq_t a2 = (seq_t)2;   /* a(n-3) */
    seq_t a3 = (seq_t)3;   /* a(n-2) */
    seq_t a4 = (seq_t)12;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = a4 + 4 * a3 + 3 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
