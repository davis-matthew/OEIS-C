/*
 * A015492  a(0)=1, a(1)=6, a(n) = sum_{k=0}^{k=n-1} 6^k a(k).
 * https://oeis.org/A015492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{n-1} 6^k a(k) with a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n>=3: a(n) = a(n-1) * (1 + 6^(n-1)) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)37;   /* 1 + 6*6 */

    /* Start from a(2) and compute forward */
    seq_t a_prev = (seq_t)37;        /* a(2) */
    seq_t pow6   = (seq_t)36;        /* 6^(3-1) = 6^2 */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t term   = a_prev * ((seq_t)1 + pow6); /* a(i-1) * (1 + 6^(i-1)) */
        a_prev = term;
        pow6  *= (seq_t)6;               /* advance 6^(i-1) -> 6^i */
    }
    return a_prev;
}
