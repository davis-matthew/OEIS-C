/*
 * A190598  Maximal digit in base-9 expansion of n.
 * https://oeis.org/A190598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = maximal base-9 digit of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t max_digit = 0;
    idx_t x = n;

    while (x > 0) {
        uint64_t d = x % 9;
        if ((seq_t)d > max_digit)
            max_digit = (seq_t)d;
        x /= 9;
    }

    return max_digit;
}
