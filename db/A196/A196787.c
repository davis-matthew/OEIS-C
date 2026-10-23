/*
 * A196787  a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5) with initial terms 1,
 *          1, 1, 3, 6.
 * https://oeis.org/A196787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = a(3) = 1; * a(4) = 3, a(5) = 6 * a(n) = a(n-1) +
 *            a(n-2) + a(n-3) + (n-4) for n >= 6 * This recurrence follows
 *            from the original * a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)
 *            with the same * initial terms, but is easier to compute
 *            efficiently
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 1 || n == 2 || n == 3)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)3;
    if (n == 5)
        return (seq_t)6;

    /* For n >= 6 we use the linear recurrence */
    seq_t prev1 = (seq_t)6;   /* a(n-1)   when k==6  */
    seq_t prev2 = (seq_t)3;   /* a(n-2)   when k==6  */
    seq_t prev3 = (seq_t)1;   /* a(n-3)   when k==6  */
    seq_t current = 0;

    /* Iterate from n = 6 up to the requested n. */
    for (idx_t k = 6; k <= n; ++k) {
        current = prev1 + prev2 + prev3 + (seq_t)(k - 4);
        /* Shift the window of the last three terms. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
