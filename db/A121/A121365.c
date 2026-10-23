/*
 * A121365  a(n) = 6*a(n-1) - 9*a(n-2) + n + 1.
 * https://oeis.org/A121365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = a(2) = a(3) = 1,
   for n >= 3: a(n) = 6*a(n-1) - 9*a(n-2) + n + 1
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 1 || n == 2 || n == 3)
        return (seq_t)1;

    /* Smallest two terms needed to start the recurrence */
    seq_t a_prev_prev = (seq_t)1; /* a(1) */
    seq_t a_prev     = (seq_t)1; /* a(2) */
    seq_t a_curr = 0;

    /* Iterate from n = 3 up to the requested n */
    for (idx_t i = 3; i <= n; ++i)
    {
        a_curr = 6 * a_prev - 9 * a_prev_prev + (seq_t)(i + 1);
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }

    return a_curr;
}
