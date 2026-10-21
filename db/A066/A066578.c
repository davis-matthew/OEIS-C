/*
 * A066578  a(n) = floor(n/(sum of digits of n)).
 * https://oeis.org/A066578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n/(sum of digits of n))
 * Range:     u8 n <= 292, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for 0, but return 0 safely */
    idx_t sum = 0;
    idx_t x = n;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    /* sum is always >= 1 for n >= 1 */
    return (seq_t)n / (seq_t)sum;
}
