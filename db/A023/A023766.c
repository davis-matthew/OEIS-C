/*
 * A023766  Nialpdromes: digits in base 11 are in nonincreasing order.
 * https://oeis.org/A023766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_ui
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_ui.h"

/* Enumeration of base‑11 numbers whose digits are nonincreasing. */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* a(1) = 0 */

    /* remaining is the 1‑based index among numbers with a nonzero leading digit */
    idx_t remaining = n - 1;   /* skip the zero term */
    idx_t length = 0, count_len;

    /* Find the length of the number */
    while (1) {
        ++length;
        count_len = binom_ui(length + 10, 10) - 1; /* numbers of exactly this length */
        if (remaining <= count_len) break;
        remaining -= count_len;
    }

    /* Zero‑based index within numbers of this length */
    idx_t idx0 = remaining - 1;

    /* Store the digits (most significant first) */
    uint8_t digits[64];
    idx_t prev = 10;    /* maximum allowed digit for the current position */

    /* Choose the most significant digit */
    {
        idx_t r = length - 1;          /* remaining positions after current */
        for (idx_t d = 1; d <= 10; ++d) {
            idx_t cnt = binom_ui(r + d, r);
            if (idx0 >= cnt) {
                idx0 -= cnt;
                continue;
            }
            digits[0] = (uint8_t)d;
            prev = d;
            break;
        }
    }

    /* Choose the remaining digits */
    for (idx_t pos = 1; pos < length; ++pos) {
        idx_t r = length - pos - 1;    /* remaining positions after this one */
        for (idx_t d = 0; d <= prev; ++d) {
            idx_t cnt = binom_ui(r + d, r);
            if (idx0 >= cnt) {
                idx0 -= cnt;
                continue;
            }
            digits[pos] = (uint8_t)d;
            prev = d;
            break;
        }
    }

    /* Convert base‑11 digits to decimal value */
    seq_t value = 0;
    for (idx_t i = 0; i < length; ++i)
        value = value * (seq_t)11 + (seq_t)digits[i];

    return value;
}
