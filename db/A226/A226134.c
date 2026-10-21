/*
 * A226134  The partial digital sums of n from left to right mod 10 give the
 *          digits of a(n).
 * https://oeis.org/A226134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the integer whose decimal digits are the cumulative
 *            partial sums of the decimal digits of n taken modulo 10. That
 *            is, if n has digits d_k … d_1 (most to least significant), let
 *            s_0 = 0 and s_i = (s_{i-1}+d_i) mod 10. Then a(n) is the number
 *            with digits s_k … s_1
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* At most 20 decimal digits for a 64‑bit index. */
    uint8_t digits[20];
    size_t len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    seq_t result = 0;
    seq_t csum   = 0;               /* running sum modulo 10 */

    /* Process digits from most significant to least. */
    for (size_t i = len; i > 0; --i) {
        seq_t d = (seq_t)digits[i - 1];
        csum   = (csum + d) % (seq_t)10;
        result = result * (seq_t)10 + csum;
    }
    return result;
}
