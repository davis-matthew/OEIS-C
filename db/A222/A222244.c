/*
 * A222244  In the number n, replace all (decimal) digits '4' with '9' and
 *          vice versa.
 * https://oeis.org/A222244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   replace decimal digit 4 with 9 and digit 9 with 4 in n
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t multiplier = 1;
    idx_t temp = n;

    while (temp > 0) {
        unsigned digit = temp % 10;
        if (digit == 4) {
            digit = 9;
        } else if (digit == 9) {
            digit = 4;
        }
        result += (seq_t)digit * multiplier;
        multiplier *= 10;
        temp /= 10;
    }
    return result;
}
