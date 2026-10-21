/*
 * A097153  Reversal of n minus largest digit of n.
 * https://oeis.org/A097153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = rev(n) - max_digit(n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rev = 0;
    seq_t maxdig = 0;
    uint64_t tmp = n;

    while (tmp > 0) {
        uint64_t digit = tmp % 10;
        rev = rev * 10 + (seq_t)digit;
        if ((seq_t)digit > maxdig) {
            maxdig = digit;
        }
        tmp /= 10;
    }
    return rev - maxdig;
}
