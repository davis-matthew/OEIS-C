/*
 * A163733  Number of n X 2 binary arrays with all 1's connected, all corners
 *          1, and no 1 having more than two 1's adjacent.
 * https://oeis.org/A163733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=2, a(4)=2; for n>=5, a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A163733 by this recurrence.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)2;

    /* For n >= 5: iterative computation using the Fibonacci-like recurrence. */
    seq_t prev2 = (seq_t)2;   /* a(3) */
    seq_t prev1 = (seq_t)2;   /* a(4) */
    seq_t next  = 0;

    idx_t i;
    for (i = 5; i <= n; ++i)
    {
        next   = prev1 + prev2; /* a(i) = a(i-1) + a(i-2) */
        prev2  = prev1;
        prev1  = next;
    }

    return prev1;
}
