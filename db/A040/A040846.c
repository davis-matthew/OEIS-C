/*
 * A040846  Continued fraction for sqrt(876).
 * https://oeis.org/A040846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=29; a(n)=period[(n-1)%8] for n>0 where
 *            period={1,1,2,14,2,1,1,58}. Continued fraction for sqrt(876)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    if (n == 0)
        return 29;          /* floor(sqrt(876)) */

    static const seq_t period[8] = {1, 1, 2, 14, 2, 1, 1, 58};

    return period[(size_t)((n - 1) % 8)];
}
