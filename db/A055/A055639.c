/*
 * A055639  Number of nonzero digits of n which are not factors of n.
 * https://oeis.org/A055639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of nonzero decimal digits d of n such that n % d
 *            != 0
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;
    idx_t temp = n;
    while (temp > (idx_t)0)
    {
        int digit = (int)(temp % (idx_t)10);
        temp /= (idx_t)10;
        if (digit != 0 && (n % (idx_t)digit) != (idx_t)0)
            count += (seq_t)1;
    }
    return count;
}
