/*
 * A095448  a(n) = 28 written in base n.
 * https://oeis.org/A095448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = integer obtained by writing the decimal number 28 in base
 *            n and concatenating the base‑n digits as decimal digits. For n =
 *            1 (unary) the value is 28 ones
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 14, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Unary case: 28 ones */
    if (n == 1) {
        seq_t v = 0;
        for (int i = 0; i < 28; ++i)
            v = v * 10 + 1;
        return v;
    }

    /* Extract base‑n digits of 28, least significant first */
    idx_t m = 28;
    int digits[64];
    int len = 0;
    while (m > 0) {
        idx_t r = m % n;      /* r < n, at most 28 */
        digits[len++] = (int)r;
        m = m / n;
    }

    /* Build the decimal integer from most significant digit */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d < 10)
            result = result * 10 + d;
        else /* 10 <= d <= 28 */
            result = result * 100 + d;   /* two decimal digits */
    }
    return result;
}
