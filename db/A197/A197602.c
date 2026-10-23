/*
 * A197602  a(n) = floor((n + 1/n)^3).
 * https://oeis.org/A197602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/n)^3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;          // n as seq_t
    seq_t n2 = m * m;            // n^2
    seq_t n3 = n2 * m;           // n^3
    seq_t three_n = m * 3;       // 3*n
    seq_t floor3 = 3 / m;        // floor(3/n)
    seq_t result = n3 + three_n + floor3;
    if (n == 1)                  // add the 1/n^3 part when n==1
        result += 1;
    return result;
}
