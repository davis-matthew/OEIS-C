/*
 * A163073  a(n) = ((5+sqrt(5))*(4+sqrt(5))^n + (5-sqrt(5))*(4-sqrt(5))^n)/10.
 * https://oeis.org/A163073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A163073
 * Formula: a(n) = ((5+√5)*(4+√5)^n + (5-√5)*(4-√5)^n)/10
 * Recurrence: a(0)=1, a(1)=5, a(n)=8*a(n-1)-11*a(n-2) for n>1
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Use a simple iterative implementation of the linear recurrence.
       Only two previous values are needed, so no dynamic memory is used. */
    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)5; /* a(n-1) */
    seq_t cur   = 0;        /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)8 * prev1 - (seq_t)11 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
