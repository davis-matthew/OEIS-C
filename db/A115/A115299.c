/*
 * A115299  Greatest digit of n + least digit of n. Different from A088133.
 * https://oeis.org/A115299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = greatest digit of n + least digit of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9043 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t max_min_digit_sum(idx_t n)
{
    uint64_t num = n;          /* copy the index value */
    uint8_t max = 0, min = 9;  /* digits are 0‑9 */

    /* Extract decimal digits */
    while (num > 0) {
        uint8_t d = num % 10;
        if (d > max) max = d;
        if (d < min) min = d;
        num /= 10;
    }

    return (seq_t)(max + min);
}

seq_t a(idx_t n)
{
    /* Sequence is undefined for n == 0, but offset is 1 */
    return max_min_digit_sum(n);
}
