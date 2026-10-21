/*
 * A095590  a(n) = 99 written in base n.
 * https://oeis.org/A095590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal number formed by writing 99 in base n and
 *            interpreting the resulting digits as decimal digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_A095590_H
#define SEQ_A095590_H
#include <stdint.h>

/* The function uses seq_t and idx_t which are provided by the build system.
   seq_t might be an unsigned integer type or an arbitrary-precision class.
   idx_t is unsigned integer (uint64_t). */
seq_t a(idx_t n)
{
    /* Handle the special base‑1 case.  In base 1, 99 is represented by
       99 copies of the digit 1.  We build it by repeated
       multiplication by 10 and adding 1. */
    if (n == 1) {
        seq_t result = 0;
        /* 99 iterations of *10+1 produce 111…1 (99 digits). */
        for (idx_t i = 0; i < 99; ++i) {
            result = result * (seq_t)10 + (seq_t)1;
        }
        return result;
    }

    /* For bases n >= 2, convert 99 to base n.  The most significant
       digit ends up in the highest position.  At most 7 digits are
       possible because 2^7 > 99.  We use a small fixed buffer. */
    idx_t num = (idx_t)99;
    int digits[16];
    int dcnt = 0;
    while (num > 0) {
        digits[dcnt++] = (int)(num % n);
        num /= n;
    }

    /* Build the decimal value.  Each digit may require up to four
       decimal digits (when the base is large).  We multiply the
       running total by 10^k and add the digit. */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        int d = digits[i];
        if (d < 10) {
            result = result * (seq_t)10 + (seq_t)d;
        } else if (d < 100) {
            result = result * (seq_t)100 + (seq_t)d;
        } else if (d < 1000) {
            result = result * (seq_t)1000 + (seq_t)d;
        } else {
            result = result * (seq_t)10000 + (seq_t)d;
        }
    }
    return result;
}
#endif
