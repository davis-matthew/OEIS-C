/*
 * A215887  Written in decimal, n ends in a(n) consecutive nonzero digits.
 * https://oeis.org/A215887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of trailing non‑zero decimal digits of n
 * Range:     u8 n <= 9857, u16 n <= 9857, u32 n <= 9857, u64 n <= 9857,
 *            u128 n <= 9857, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    idx_t t = n;
    while (t % 10ULL != 0ULL) {
        cnt++;
        t /= 10ULL;
    }
    return cnt;
}
