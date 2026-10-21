/*
 * A095476  a(n) = 42 written in base n.
 * https://oeis.org/A095476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        /* 42 in base 1 = 42 ones → (10^42 – 1)/9 */
        seq_t pow10 = 1;
        for (int i = 0; i < 42; ++i) {
            pow10 *= 10;
        }
        seq_t num = pow10 - 1;
        return num / 9;
    }

    /* Convert 42 to base n, storing digits least‑significant first. */
    uint64_t m = 42ULL;
    uint8_t digits[64];
    idx_t len = 0;
    while (m > 0) {
        uint64_t d = m % n;
        m /= n;
        digits[len++] = (uint8_t)d;   /* d is always ≤ 42 */
    }

    /* Construct the concatenated decimal integer from the base‑n digits. */
    seq_t result = 0;
    idx_t i = len;
    while (i--) {
        uint8_t d = digits[i];
        seq_t shift = (d < 10 ? 10 : 100);
        result = result * shift + (seq_t)d;
    }
    return result;
}
