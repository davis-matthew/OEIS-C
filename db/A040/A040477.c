/*
 * A040477  Continued fraction for sqrt(500).
 * https://oeis.org/A040477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = continued fraction expansion of sqrt(500); a(0)=22; for
 *            n>0, a(n) equals period[(n-1)%14] where period is
 *            (2,1,3,2,1,1,10,1,1,2,3,1,2,44)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t period[14] = {
        2, 1, 3, 2, 1, 1, 10, 1, 1, 2, 3, 1, 2, 44
    };
    if (n == 0)
        return 22;
    idx_t idx = (n - 1) % 14;
    return period[idx];
}
