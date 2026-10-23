/*
 * A030106  Base 7 reversal of n (written in base 10).
 * https://oeis.org/A030106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n)=“reverse the digits of the base‑7 representation of n and
 *            interpret as a base‑7 number”
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t tmp = n;
    uint8_t digits[32];
    int cnt = 0;

    /* obtain base‑7 digits (least‑significant first) */
    while (tmp > 0) {
        digits[cnt++] = tmp % 7;
        tmp /= 7;
    }

    /* build result from reversed digits */
    seq_t res = 0;
    for (int i = 0; i < cnt; ++i) {
        res = res * 7 + (seq_t)digits[i];
    }

    return res;
}
