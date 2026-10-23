/*
 * A122586  Leading digit of n expressed in base 3.
 * https://oeis.org/A122586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = floor(n / (3^floor(log_3(n)))) (leading digit in base 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) is undefined for the sequence; return 0 as a harmless sentinel. */
        return 0;
    }
    idx_t x = n;
    while (x >= 3) {
        x /= 3;          /* strip trailing trits until the most significant one is left */
    }
    return (seq_t)x;     /* 1 or 2 */
}
