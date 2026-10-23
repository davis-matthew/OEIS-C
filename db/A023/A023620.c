/*
 * A023620  Convolution of Lucas numbers and odd numbers.
 * https://oeis.org/A023620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4)
   with initial values a(1)=1, a(2)=6, a(3)=18, a(4)=41 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)18;
    if (n == 4) return (seq_t)41;

    /* maintain the four previous terms */
    seq_t a_prev4 = (seq_t)1;   /* a(i-4) */
    seq_t a_prev3 = (seq_t)6;   /* a(i-3) */
    seq_t a_prev2 = (seq_t)18;  /* a(i-2) */
    seq_t a_prev1 = (seq_t)41;  /* a(i-1) */
    seq_t cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* compute a(i) = 3*a_prev1 - 2*a_prev2 - a_prev3 + a_prev4
           using only unsigned arithmetic. */
        seq_t tmp = a_prev4 + a_prev1 * (seq_t)3;
        tmp -= a_prev2 * (seq_t)2;
        tmp -= a_prev3;
        cur = tmp;

        /* shift the window of previous terms */
        a_prev4 = a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return cur;
}
