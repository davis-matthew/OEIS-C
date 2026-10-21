/*
 * A222242  In the number n, replace all (decimal) digits '4' with '7' and
 *          vice versa.
 * https://oeis.org/A222242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal digit map 4↔7 in the decimal representation of n
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
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

    /* buffer to store digits in reverse order (max 20 digits for 64‑bit n) */
    uint8_t buf[21];
    int len = 0;
    idx_t tmp = n;

    /* extract digits from least significant to most significant */
    while (tmp > 0) {
        buf[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t result = 0;
    /* process digits from most significant to least,
       applying the 4↔7 map and accumulating the decimal number */
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = buf[i];
        uint8_t m = (d == 4 ? 7 : (d == 7 ? 4 : d));
        result = result * 10 + m;
    }
    return result;
}
