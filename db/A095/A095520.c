/*
 * A095520  a(n) = 64 written in base n.
 * https://oeis.org/A095520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = write the decimal number 64 in base n and interpret *
 *            the base‑n digits as a decimal (base‑10) integer. * Special
 *            cases: * n = 1 -> unary representation: 64 ones => decimal value
 *            with 64 digits '1'. * n ≥ 65 -> since 64 < n, 64 is a single
 *            base‑n digit: result = 64
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_A095520_H
#define SEQ_A095520_H

#include <stddef.h>  // for size_t (used by seq_t compatibility)

seq_t a(idx_t n)
{
    /* Ensure n is at least 1; behaviour for n==0 is undefined. */
    /* Unary representation: 64 ones. */
    if (n == 1) {
        seq_t val = 0;
        for (int i = 0; i < 64; ++i) {
            val = val * 10 + 1;
        }
        return val;
    }

    /* For bases larger than 64 the number 64 is a single digit. */
    if (n >= 65) {
        return (seq_t)64;
    }

    /* n is between 2 and 64 inclusive.  Compute the base‑n digits of 64. */
    idx_t temp = 64;
    /* 64 in binary has 7 digits, so at most 7 digits in any base >=2. */
    int digits[7];
    int len = 0;
    do {
        digits[len++] = (int)(temp % n);
        temp /= n;
    } while (temp > 0);

    /* Build the decimal integer by concatenating the decimal
     * representations of the individual base‑n digits. */
    seq_t val = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        /* The decimal width of d is 1 if d < 10, else 2 (since d ≤ 63). */
        seq_t pow10 = (d < 10) ? (seq_t)10 : (seq_t)100;
        val = val * pow10 + (seq_t)d;
    }

    return val;
}

#endif  /* SEQ_A095520_H */
