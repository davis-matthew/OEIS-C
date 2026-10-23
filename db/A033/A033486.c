/*
 * A033486  a(n) = n*(n + 1)*(n + 2)*(n + 3)/2.
 * https://oeis.org/A033486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n + 2)*(n + 3)/2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 301, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t res = n0 * (n0 + 1) * (n0 + 2) * (n0 + 3) / 2;
    return res;
}
