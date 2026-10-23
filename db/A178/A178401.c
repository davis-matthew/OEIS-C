/*
 * A178401  Number of times the rounded up arithmetic mean of digits of n
 *          occurs in n, cf. A004427.
 * https://oeis.org/A178401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of times the ceiling of the arithmetic mean of the
 *            decimal digits of n occurs in n
 * Range:     u8 n <= 9557, u16 n <= 9557, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Extract digits of n in base 10.  For n == 0 we need a single digit 0. */
    uint8_t digits[20];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Compute the sum of digits. */
    seq_t sum = 0;
    for (int i = 0; i < len; ++i) {
        sum += (seq_t)digits[i];
    }

    /* Compute the ceiling of the mean: ceil(sum / len). */
    seq_t ceil_mean = (sum + (seq_t)len - 1) / (seq_t)len;

    /* Count how many digits equal ceil_mean. */
    seq_t count = 0;
    for (int i = 0; i < len; ++i) {
        if ((seq_t)digits[i] == ceil_mean) {
            ++count;
        }
    }

    return count;
}
