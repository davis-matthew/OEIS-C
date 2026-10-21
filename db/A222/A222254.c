/*
 * A222254  In the number n, replace all (decimal) digits '8' with '9' and
 *          vice versa.
 * https://oeis.org/A222254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace digits 8<->9 in the decimal representation of n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
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

    seq_t result = 0;
    uint8_t digs[20];
    int dlen = 0;
    uint64_t tmp = (uint64_t)n;

    /* Extract digits into a buffer (least‑significant first) */
    while (tmp > 0) {
        digs[dlen++] = tmp % 10;
        tmp /= 10;
    }

    /* Build the new number from most‑significant digit to least */
    for (int i = dlen - 1; i >= 0; --i) {
        unsigned digit = digs[i];
        unsigned mapped = (digit == 8 ? 9
                          : (digit == 9 ? 8 : digit));
        result = result * 10 + mapped;
    }

    return result;
}
