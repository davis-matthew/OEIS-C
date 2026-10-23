/*
 * A037892  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*6^i} is the base 6
 *          representation of n and e(i) are the digits d(i) in reverse order.
 * https://oeis.org/A037892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1/2 * sum_{i} |d(i) - e(i)|, where * d(i) are the base-6
 *            digits of n (least significant first) * and e(i) are those
 *            digits in reverse order. * This is equivalent to summing |d(i) -
 *            d(k-1-i)| over the first half * of the digit string
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* offset starts at 1, but guard 0 */

    /* Base-6 digits of n: least significant first.  n fits in 64 bits,
       so at most 25 digits. */
    uint8_t digits[64];
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp > 0u) {
        digits[len++] = tmp % 6u;
        tmp /= 6u;
    }

    seq_t sum = 0;
    for (idx_t i = 0; i < len / 2; ++i) {
        uint8_t a = digits[i];
        uint8_t b = digits[len - 1 - i];
        sum += a > b ? a - b : b - a;
    }
    return sum;
}
