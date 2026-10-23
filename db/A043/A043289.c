/*
 * A043289  Maximal run length in base-15 representation of n.
 * https://oeis.org/A043289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the maximal length of a run of equal digits in the
 *            base‑15 representation of n
 * Range:     u8 n <= 9362, u16 n <= 9362, u32 n <= 9362, u64 n <= 9362,
 *            u128 n <= 9362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to base 15 and store the digits. */
    uint8_t digits[32];   /* 32 >> 16 digits are enough for any 64‑bit n. */
    size_t len = 0;
    idx_t temp = n;

    while (temp > 0) {
        digits[len++] = temp % 15;
        temp /= 15;
    }

    /* Find the longest run of equal consecutive digits in the
       original (most‑significant first) order. */
    unsigned maxrun = 1;
    unsigned currun = 1;

    for (size_t i = len - 1; i > 0; --i) {
        if (digits[i] == digits[i - 1]) {
            ++currun;
            if (currun > maxrun)
                maxrun = currun;
        } else {
            currun = 1;
        }
    }

    return (seq_t)maxrun;
}
