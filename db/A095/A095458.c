/*
 * A095458  a(n) = 33 written in base n.
 * https://oeis.org/A095458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of 33 in base n, written in decimal
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 16, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* Special case: base‑1 (unary) representation */
    if (n == 1) {
        seq_t r = 0;
        for (idx_t i = 0; i < 33; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* For n > 33 the base‑n representation of 33 is a single digit 33,
       so the decimal value is 33. */
    if (n > 33)
        return (seq_t)33;

    /* Compute the base‑n digits of 33. 33 is small, so a fixed array is enough. */
    seq_t digits[10];
    idx_t dcnt = 0;
    seq_t num = (seq_t)33;

    while (num > 0) {
        digits[dcnt++] = num % n;
        num /= n;
    }

    /* Concatenate the digits to form the decimal number. */
    seq_t res = 0;
    for (idx_t i = dcnt; i > 0; --i)
        res = res * 10 + digits[i - 1];

    return res;
}
