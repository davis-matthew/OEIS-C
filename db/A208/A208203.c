/*
 * A208203  a(n) = (a(n-1)*a(n-2)^3+1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)*a(n-2)^3 + 1)/a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial terms */
    if (n < 3)
        return (seq_t)1;

    /* Keep the last three values of the recurrence.
       a0 = a(n-3), a1 = a(n-2), a2 = a(n-1). */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1;

    for (idx_t i = (idx_t)3; i <= n; ++i)
    {
        /* a1^3 */
        seq_t cube = a1 * a1;
        cube *= a1;

        /* numerator = a2 * cube + 1 */
        seq_t num = a2 * cube;
        num += (seq_t)1;

        /* Compute the next term */
        seq_t a3 = num / a0;

        /* Shift the window: a0 <- a1, a1 <- a2, a2 <- a3 */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
