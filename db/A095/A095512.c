/*
 * A095512  a(n) = 60 written in base n.
 * https://oeis.org/A095512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the digits of 60 written in base
 *            n (base‑1 is the unary “60 ones”)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* Base‑1 (unary) representation: 60 ones. */
    if (n == 1) {
        seq_t res = 0;
        /* Append a '1' 60 times. */
        for (idx_t i = 0; i < 60; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* For n > 60, 60 < n so it is a single digit “60”. */
    if (n > 60)
        return (seq_t)60;

    /* Normal base‑n conversion for 2 ≤ n ≤ 60. */
    seq_t base = (seq_t)n;
    seq_t num  = (seq_t)60;

    /* Array to hold digits of 60 in base n (least‑significant first).
       For n = 2, 60 requires at most 60 digits, so 64 is safe. */
    seq_t digits[64];
    idx_t dcnt = 0;

    while (num > 0) {
        digits[dcnt++] = num % base;
        num /= base;
    }

    /* Convert the digit array to the final decimal integer by
       concatenating the decimal representations of each digit. */
    seq_t result = 0;

    for (idx_t i = dcnt; i-- > 0;) {          /* iterate most‑significant to least */
        seq_t d = digits[i];

        /* Determine the number of decimal digits of d. */
        idx_t len = 0;
        seq_t tmp = d;
        do {
            ++len;
            tmp /= (seq_t)10;
        } while (tmp);

        /* Compute 10^len. */
        seq_t pow10 = 1;
        for (idx_t j = 0; j < len; ++j)
            pow10 *= (seq_t)10;

        result = result * pow10 + d;
    }

    return result;
}
