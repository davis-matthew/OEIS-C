/*
 * A061282  Minimal number of steps to get from 0 to n by (a) adding 1 or (b)
 *          multiplying by 3. A stopping problem: begin with n and at each
 *          stage if a multiple of 3 divide by 3, otherwise subtract 1.
 * https://oeis.org/A061282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (number of base‑3 digits of n) + (sum of base‑3 digits of
 *            n) - 1 (a(0)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t digits = 0;
    seq_t sum = 0;
    idx_t temp = n;

    while (temp > 0)
    {
        sum += (seq_t)(temp % 3);
        temp /= 3;
        digits++;
    }
    return digits + sum - 1;
}
