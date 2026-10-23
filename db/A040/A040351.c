/*
 * A040351  Continued fraction for sqrt(371).
 * https://oeis.org/A040351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=19; for n ≥ 1, a(n) = a(n-6) (n ≥ 7) with period
 *            3,1,4,1,3,38
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;

    static const seq_t period[6] = {
        (seq_t)3, (seq_t)1, (seq_t)4, (seq_t)1, (seq_t)3, (seq_t)38
    };

    idx_t idx = (n - 1) % 6;   /* 0 .. 5 */
    return period[(size_t)idx];
}
