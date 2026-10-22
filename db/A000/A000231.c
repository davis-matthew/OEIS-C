/*
 * A000231  Number of inequivalent Boolean functions of n variables under
 *          action of complementing group.
 * https://oeis.org/A000231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (2^(2^n)+(2^n-1)*2^(2^(n-1)))/2^n = 2^(2^n-n) +
 *            (2^n-1)*2^(2^(n-1)-n) (n >= 1), a(0) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t one = 1;
    if (n == 0)
        return (seq_t)2;
    uint64_t N = (uint64_t)1 << n;          /* 2^n */
    uint64_t e1 = N - n;                    /* 2^n - n */
    uint64_t e2 = (N >> 1) - n;             /* 2^(n-1) - n, >= 0 for n >= 1 */
    seq_t t1 = one << e1;
    seq_t t2 = ((one << n) - 1) * (one << e2);
    return t1 + t2;
}
