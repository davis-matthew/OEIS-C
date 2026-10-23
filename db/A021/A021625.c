/*
 * A021625  Decimal expansion of 1/621.
 * https://oeis.org/A021625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 1/621 (period 66). This implementation
   computes the digits by repeated long division and caches the period for
   all subsequent calls.  For any n >= 0, a(n) returns the nth digit after
   the decimal point of 1/621. */

#include <stddef.h>
#include <stdbool.h>

static seq_t digits[66];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized)
    {
        uint64_t r = 1;                 /* current remainder */
        for (size_t i = 0; i < 66; ++i)
        {
            r = r * 10;
            seq_t d = (seq_t)(r / 621);  /* digit 0‑9 */
            digits[i] = d;
            r = r % 621;
        }
        initialized = true;
    }
    size_t pos = (size_t)(n % 66);
    return digits[pos];
}
