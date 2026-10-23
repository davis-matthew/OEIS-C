/*
 * A199109  a(n) = (7*3^n + 1)/2.
 * https://oeis.org/A199109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*3^n + 1)/2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Use the simple linear recurrence a(n) = 3*a(n-1) - 1 for n >= 1,
       with a(0) = 4.  All intermediate values stay within the limits of seq_t
       whenever the final result does.  */
    if (n == 0)
        return (seq_t)4;

    seq_t val = (seq_t)4;       /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        val = val * (seq_t)3 - (seq_t)1;   /* a(i) */

    return val;
}
