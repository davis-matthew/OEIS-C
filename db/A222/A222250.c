/*
 * A222250  In the number n, replace all (decimal) digits '6' with '8' and
 *          vice versa.
 * https://oeis.org/A222250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Map: replace decimal digits 6 and 8 (swap 6↔8), others unchanged. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t temp = n;
    idx_t pow10 = 1;
    /* find the highest power of 10 <= n */
    while (pow10 <= temp / 10)
        pow10 *= 10;

    seq_t result = 0;
    while (pow10 > 0) {
        idx_t digit = temp / pow10;
        temp %= pow10;
        pow10 /= 10;
        int mapped = (int)digit;
        if (digit == 6)
            mapped = 8;
        else if (digit == 8)
            mapped = 6;
        result = result * 10 + (seq_t)mapped;
    }
    return result;
}
