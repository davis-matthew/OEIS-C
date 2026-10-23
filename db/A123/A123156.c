/*
 * A123156  a(n) = 10*n in base n+1.
 * https://oeis.org/A123156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal representation of (10*n) written in base (n+1)
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function converts 10*n to base (n+1) and concatenates the digits
   (each digit is treated as a decimal number).  This yields the sequence
   of OEIS A123156.  For n ≥ 1, all operations are performed on seq_t
   values, so the implementation works for any seq_t width or an arbitrary
   precision integer type. */

#include "../utils/pow10.h"

static int decimal_digits(seq_t d)
{
    /* Return the number of decimal digits of d (d > 0).  For d == 0
       the result is 1. */
    int cnt = 0;
    do {
        d /= 10;
        ++cnt;
    } while (d != 0);
    return cnt;
}

seq_t a(idx_t n)
{
    /* Argument is 1‑based; for n < 1 behaviour is undefined. */
    seq_t base = (seq_t)(n + 1);
    seq_t value = (seq_t)10 * (seq_t)n;

    /* Convert value to base 'base', storing digits in an array. */
    static seq_t digits[128];
    int len = 0;
    seq_t tmp = value;
    while (tmp != 0) {
        digits[len++] = tmp % base;   /* obtain least‑significant digit */
        tmp /= base;
    }

    /* Assemble the decimal concatenation of the digits. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        seq_t d = digits[i];
        int dec_len = decimal_digits(d);
        seq_t pow10_val = pow10((idx_t)dec_len);
        result = result * pow10_val + d;
    }
    return result;
}
