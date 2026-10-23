/*
 * A193238  Number of prime digits in decimal representation of n.
 * https://oeis.org/A193238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of prime digits (2,3,5,7) in the decimal
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    while (n > 0)
    {
        unsigned int d = n % 10U;
        if (d == 2U || d == 3U || d == 5U || d == 7U)
            cnt++;
        n /= 10U;
    }
    return cnt;
}
