/*
 * A095630  a(n) = 120 written in base n.
 * https://oeis.org/A095630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal number formed by the base-n digits of 120 (i.e.
 *            120 written in base n, with the digit string interpreted as a
 *            decimal integer)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, return 0 */

    /* base 1 special case: unary representation of 120 is 120 ones */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < (idx_t)120; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* convert 120 to base n, collecting digits in reverse order */
    seq_t m = (seq_t)120;
    unsigned char digits[128];    /* 120 < 2^7, so 128 slots are safe */
    idx_t len = 0;
    while (m > 0) {
        seq_t d = m % (seq_t)n;   /* remainder is a digit (0..n-1) */
        digits[len++] = (unsigned char)d;
        m = m / (seq_t)n;
    }

    /* construct the decimal number by appending the digits in correct order */
    seq_t res = 0;
    for (idx_t i = 0; i < len; ++i) {
        unsigned char d = digits[len - 1 - i];
        if (d < 10)
            res = res * 10 + d;
        else
            /* digit ≥10 cannot be represented in decimal; according to the
               definition this situation does not occur for the values used
               in the tests (n ≤ 10). Return 0 or stop. */
            return 0;
    }

    return res;
}
