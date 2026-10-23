/*
 * A021487  Decimal expansion of 1/483.
 * https://oeis.org/A021487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/483 (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Periodic digits of 1/483. The true period length is 66. */
static seq_t period_digits[66];
static idx_t period_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Lazily compute the period once. */
    if (!initialized) {
        uint64_t rem = 1;
        idx_t i = 0;
        do {
            rem *= 10;
            period_digits[i] = (seq_t)(rem / 483U);
            rem %= 483U;
            ++i;
        } while (rem != 1U);
        period_len = i;
        initialized = true;
    }

    /* The sequence repeats with period_len. */
    idx_t idx = n % period_len;
    return period_digits[idx];
}
