/*
 * A061879  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 10.
 * https://oeis.org/A061879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 2328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9860 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061879: Numbers whose alternating digit sum equals 10. */

#include <stdint.h>

/* Compute the alternating digit sum d₁ – d₂ + d₃ – d₄ + … for a decimal number. */
static int alt_sum_idx(idx_t n)
{
    int sum = 0;
    idx_t temp = n;
    int digits[20];
    idx_t len = 0;

    /* Extract digits, least significant first */
    do {
        digits[len++] = temp % 10;
        temp /= 10;
    } while (temp > 0);

    /* Sum from leftmost digit: even positions add, odd positions subtract. */
    for (idx_t i = 0; i < len; ++i) {
        idx_t pos = len - 1 - i;    /* leftmost position, 0‑based */
        int d = digits[i];
        if ((pos & 1) == 0)
            sum += d;    /* + sign */
        else
            sum -= d;    /* – sign */
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence starts at offset 1 */

    idx_t count = 0;
    idx_t num   = 1;

    for (;; ++num) {
        if (alt_sum_idx(num) == 10) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
