/*
 * A015475  q-Fibonacci numbers for q=4, scaling a(n-1).
 * https://oeis.org/A015475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1)*a(n-1) + a(n-2) with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prevprev = (seq_t)0;   /* a(n-2) */
    seq_t a_prev     = (seq_t)1;   /* a(n-1) */
    seq_t pow4      = (seq_t)1;    /* 4^(i-1) before computing a(i) */

    /* Compute a(2) .. a(n) iteratively */
    for (idx_t i = 2; i <= n; ++i) {
        pow4 = pow4 * (seq_t)4;                /* 4^(i-1) */
        seq_t current = pow4 * a_prev + a_prevprev; /* a(i) */
        a_prevprev = a_prev;
        a_prev = current;
    }
    return a_prev;   /* a(n) */
}
