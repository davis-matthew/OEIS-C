/*
 * A093018  Natural numbers with appended Luhn mod 10 check digit.
 * https://oeis.org/A093018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10*n + d where d is the Luhn check digit that makes the
 *            weighted sum of the decimal digits of a(n) divisible by 10. The
 *            weighted sum follows the standard Luhn algorithm (doubling every
 *            second digit from the right, subtracting 9 if the double exceeds
 *            9)
 * Range:     u8 n <= 24, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t tmp = n;            /* n is unsigned 64‑bit */
    uint64_t sum = 0;         /* weighted sum, at most a few hundred */
    int i = 0;                /* position from the right, 0‑based */

    do {
        uint64_t digit = tmp % 10;
        tmp /= 10;
        uint64_t val;
        if ((i & 1) == 0) {   /* double the digit at even positions */
            val = digit * 2;
            if (val > 9) val -= 9;
        } else {
            val = digit;
        }
        sum += val;
        ++i;
    } while (tmp > 0);

    uint64_t check = (10 - (sum % 10)) % 10;
    return (seq_t)n * 10 + (seq_t)check;
}
