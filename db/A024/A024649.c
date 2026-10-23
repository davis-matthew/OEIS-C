/*
 * A024649  n written in fractional base 8/7.
 * https://oeis.org/A024649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = representation of n in fractional base 8/7 as a decimal
 *            integer (digits 0‑7). This is computed by repeatedly extracting
 *            digits with the rule: digit = x mod 8 x = (x - digit) * 7 / 8
 *            where x starts as n. The digits (least significant first) are
 *            then interpreted in base 10 to produce the result
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 79, u64 n <= 367,
 *            u128 n <= 4927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* constants for base 8/7 */
    const seq_t A = (seq_t)8, B = (seq_t)7;

    /* extract digits 0..7, stored least significant first */
    uint8_t digits[256];
    size_t len = 0;
    seq_t x = (seq_t)n;
    while (x != 0) {
        seq_t d = x % A;
        x = (x - d) * B / A;
        digits[len++] = (uint8_t)d;
        if (len == sizeof(digits))
            break;               /* safety: should not be reached for n ≤ 10000 */
    }

    /* convert digits to a decimal integer: most significant to least */
    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i-1];
    }
    return result;
}
