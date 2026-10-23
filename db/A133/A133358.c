/*
 * A133358  Decimal expansion of 46/127.
 * https://oeis.org/A133358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(46 * 10^(n+1) / 127) mod 10. Equivalent to the nth
 *            digit (0‑indexed) of the decimal expansion of 46/127. The
 *            decimal has period 42, so the value repeats every 42 terms
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The period of 46/127 in decimal representation is 42 digits. */
    static seq_t digits[42];
    static bool ready = false;
    if (!ready) {
        uint32_t r = 46;          // initial remainder for the decimal expansion
        for (idx_t i = 0; i < 42; ++i) {
            r *= 10u;              // multiply by 10 to shift to next decimal digit
            digits[i] = (seq_t)(r / 127u); // whole number part is the digit
            r = r % 127u;          // remainder after extracting the digit
        }
        ready = true;
    }
    return digits[(unsigned long)(n % 42u)];
}
