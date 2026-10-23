/*
 * A040622  Continued fraction for sqrt(648).
 * https://oeis.org/A040622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=25; for n>=1, a(n) = cycle[(n-1)%6] with cycle =
 *            {2,5,6,5,2,50}
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
        return (seq_t)25;

    static const seq_t cycle[6] = {
        (seq_t)2,
        (seq_t)5,
        (seq_t)6,
        (seq_t)5,
        (seq_t)2,
        (seq_t)50
    };
    idx_t r = (n - 1) % 6;
    return cycle[(size_t)r];
}
