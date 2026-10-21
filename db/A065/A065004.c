/*
 * A065004  100 written in base n, or -1 if the representation requires digits
 *          outside of 0 to 9.
 * https://oeis.org/A065004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, less, sign, fini, full
 * Formula:   a(n) = decimal value of the base‑n representation of 100, * or
 *            -1 if any digit ≥10 and therefore cannot be expressed with
 *            decimal * digits 0–9
 * Range:     i8 none, i16 none, i32 none, i64 none, i128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for size_t, uint64_t if needed – harmless */

seq_t a(idx_t n)
{
    /* Base 1 is special: the “unary” expansion of 100 is 100 ones. */
    if (n == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 100; ++i) {
            val = val * 10 + 1;   /* append the digit 1 */
        }
        return val;
    }

    /* For n < 2, the sequence is undefined; return -1. */
    if (n < 2) {
        return (seq_t)(-1);
    }

    /* Convert 100 (decimal) into base n.  Store the base‑n digits
     * (least‑significant first) in a small array.  If any digit is
     * ≥10 we immediately return -1. */
    idx_t value = 100;
    int digits[20];   /* far more than enough for all bases ≥ 2 */
    int d = 0;

    while (value > 0) {
        int digit = value % n;
        if (digit >= 10) {
            return (seq_t)(-1);
        }
        digits[d++] = digit;
        value /= n;
    }
    if (d == 0) {                 /* cannot happen for value=100 */
        digits[d++] = 0;
    }

    /* Build the decimal number represented by the collected digits. */
    seq_t result = 0;
    for (int i = d - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}
