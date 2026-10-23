/*
 * A040201  Continued fraction for sqrt(216).
 * https://oeis.org/A040201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=14; for n>=1 a(n)=period[(n-1) mod 6] with period =
 *            {1,2,3,2,1,28}
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
        return (seq_t)14;

    static const seq_t period[6] = {
        (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)2, (seq_t)1, (seq_t)28
    };

    size_t idx = (size_t)((n - 1) % 6);
    return period[idx];
}
