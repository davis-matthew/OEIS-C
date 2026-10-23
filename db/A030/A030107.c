/*
 * A030107  Base 8 reversal of n (written in base 10).
 * https://oeis.org/A030107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = reverse the base‑8 representation of n, interpreted as a
 *            decimal number
 * Range:     u8 n <= 67, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n) {
        seq_t digit = (seq_t)(n & 7);   /* n % 8 */
        res = (res << 3) + digit;       /* res * 8 + digit */
        n >>= 3;                        /* n / 8 */
    }
    return res;
}
