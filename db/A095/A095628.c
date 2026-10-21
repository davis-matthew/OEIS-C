/*
 * A095628  a(n) = 119 written in base n.
 * https://oeis.org/A095628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer obtained by writing the number 119 in
 *            base n (the base‑n digits are read as decimal digits and
 *            concatenated)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* ensures idx_t and seq_t are available */

seq_t a(idx_t n)
{
    /* Handle the special base 1 case: 119 in base 1 is 119 ones. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 119; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* For bases larger than 119 the representation is a single digit 119. */
    if (n > 119) {
        return (seq_t)119;
    }

    /* Compute base‑n digits of 119.  Number of digits needed is at most 7. */
    idx_t x = 119;
    int digits[8];          /* buffer for digits, LSB first */
    int cnt = 0;
    while (x > 0) {
        digits[cnt++] = (int)(x % n);
        x /= n;
    }

    /* Build the decimal number by concatenating decimal representations of
       each digit (most significant first). */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        int d = digits[i];
        seq_t mul = (d < 10) ? (seq_t)10
                : (d < 100) ? (seq_t)100
                : (seq_t)1000;  /* d <= 118, so at most 3 decimal digits */
        res = res * mul + d;
    }
    return res;
}
