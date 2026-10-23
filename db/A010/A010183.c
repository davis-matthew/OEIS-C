/*
 * A010183  Continued fraction for sqrt(118).
 * https://oeis.org/A010183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 10; for n >= 1, a(n) = [1,6,3,2,10,2,3,6,1,20][(n-1)%10]
 * Range:     u8 n <= 700, u16 n <= 700, u32 n <= 700, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)10;

    /* The period of the continued fraction for sqrt(118) is 10 */
    static const seq_t period[10] = {
        (seq_t)1, (seq_t)6, (seq_t)3, (seq_t)2,
        (seq_t)10, (seq_t)2, (seq_t)3, (seq_t)6,
        (seq_t)1, (seq_t)20
    };

    /* Compute position within the period */
    size_t pos = (size_t)((n - 1) % 10);
    return period[pos];
}
