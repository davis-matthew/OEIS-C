/*
 * A095480  a(n) = 44 written in base n.
 * https://oeis.org/A095480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the decimal integer formed by writing 44 in base n and
 *            interpreting each base‑n digit as a decimal digit (concatenated)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Special case: base 1 (unary). 44 is represented by forty‑four
       '1' digits.  Build this number by repeated multiplication by 10
       and addition of 1. */
    if (n <= 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 44; ++i) {
            val = val * (seq_t)10 + (seq_t)1;
        }
        return val;
    }

    /* Compute the digits of 44 in base n, least‑significant first. */
    seq_t temp = 44;
    /* The representation of 44 in any base >= 2 has at most 6 digits
       (since 2^6 = 64 > 44).  An array of size 8 is safe. */
    seq_t digits[8];
    int num = 0;
    while (temp > 0) {
        seq_t digit = temp % n;
        digits[num++] = (seq_t)digit;
        temp = temp / n;
    }

    /* Convert those digits to a decimal integer by concatenating
       each digit's decimal representation. */
    seq_t val = 0;
    for (int i = num - 1; i >= 0; --i) {
        seq_t d = digits[i];
        /* d can only be 0..44, so its decimal length is 1 or 2. */
        if (d < 10) {
            val = val * (seq_t)10 + d;
        } else { /* d >= 10 && d < 100 */
            val = val * (seq_t)100 + d;
        }
    }
    return val;
}
