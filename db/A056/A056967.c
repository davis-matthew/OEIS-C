/*
 * A056967  Write what is described (putting a leading zero on numbers which
 *          have an odd number of digits).
 * https://oeis.org/A056967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = for each pair (d1,d2) of the decimal representation of n
 *            (prepend a leading 0 if n has an odd number of digits), repeat
 *            digit d2 exactly d1 times and concatenate the results. If a pair
 *            has d1 = 0 it contributes nothing. The resulting decimal string
 *            is interpreted as the sequence value. For...
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    // Convert n to its decimal digits (least significant first)
    idx_t tmp = n;
    int digits_rev[20];  /* enough for 64-bit integers */
    int len = 0;
    if (tmp == 0) {
        digits_rev[0] = 0;
        len = 1;
    } else {
        while (tmp != 0) {
            digits_rev[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    seq_t result = 0;
    int idx = len - 1;  /* index of most significant digit */

    /* If the number of digits is odd, prepend a leading zero pair */
    if (len % 2 == 1) {
        int d1 = 0;
        int d2 = digits_rev[idx];
        /* d1 == 0, contributes nothing */
        idx--;  /* consume the most significant digit */
    }

    /* Process remaining pairs from most significant to least */
    while (idx > 0) {
        int d1 = digits_rev[idx];
        int d2 = digits_rev[idx - 1];
        for (int r = 0; r < d1; ++r) {
            result = result * 10 + (seq_t)d2;
        }
        idx -= 2;
    }

    return result;
}
