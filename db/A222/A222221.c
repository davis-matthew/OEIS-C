/*
 * A222221  In the number n, replace all (decimal) digits '1' with '4' and
 *          vice versa.
 * https://oeis.org/A222221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by replacing each decimal digit 1 with 4 and 4
 *            with 1 in n
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the highest power of 10 not exceeding n. */
    idx_t power = 1;
    while (n / power >= 10)
        power *= 10;

    seq_t result = 0;
    while (power) {
        unsigned digit = (unsigned)((n / power) % 10);
        unsigned swapped = (digit == 1) ? 4 :
                           (digit == 4) ? 1 : digit;
        result = result * 10 + swapped;
        power /= 10;
    }
    return result;
}
