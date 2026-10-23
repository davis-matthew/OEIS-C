/*
 * A178523  The path length of the Fibonacci tree of order n.
 * https://oeis.org/A178523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+2*F(n+1)-2, a(0)=0, a(1)=0
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (F(n) is the n-th Fibonacci number, F(0)=0, F(1)=1) */

#include <stdint.h>
#include "../utils/fib.h"

/* Return the n-th term of A178523 (path length of Fibonacci tree) */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* Variables to hold a(n-2) and a(n-1) */
    seq_t a_prev2 = (seq_t)0; /* a(0) */
    seq_t a_prev1 = (seq_t)0; /* a(1) */
    seq_t a_curr  = (seq_t)0;

    /* Iterate from i=2 up to n, computing a(i) using the recurrence */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute 2*F(i+1)-2 */
        seq_t Fi_plus1 = fib(i + 1);          /* F(i+1) */
        seq_t temp     = (seq_t)2 * Fi_plus1; /* 2*F(i+1) */
        temp -= (seq_t)2;                     /* 2*F(i+1)-2 */

        /* a(i) = a(i-1) + a(i-2) + (2*F(i+1)-2) */
        a_curr = a_prev1 + a_prev2 + temp;

        /* Shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr; /* a(n) */
}
