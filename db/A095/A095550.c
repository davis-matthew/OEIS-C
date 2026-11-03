/*
 * A095550  a(n) = 79 written in base n.
 * https://oeis.org/A095550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the decimal integer whose decimal digits are the base‑n
 *            representation (in normal digit order) of the decimal number 79
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        /* 79 ones: 111...1 (79 digits) */
        static const char *ones =
            "1111111111111111111111111111111111111111111111111111111111111111111111111111111";
        return seq_from_str(ones);
    }

    /* Compute the representation of 79 in base n */
    /* 79 < 2^7, so at most 7 digits are needed when n >= 2 */
    unsigned char digits[8];
    size_t len = 0;
    unsigned int temp = 79;

    while (temp > 0) {
        digits[len++] = temp % (unsigned int)n;
        temp /= (unsigned int)n;
    }

    /* Convert digit sequence to a decimal integer */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + digits[len - 1 - i];
    }
    return result;
}
