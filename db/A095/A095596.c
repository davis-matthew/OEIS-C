/*
 * A095596  a(n) = 103 written in base n.
 * https://oeis.org/A095596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal value of 103 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the base‑n representation of 103 and interpret that
   representation as a decimal number.  Base 1 is treated
   specially: 103 is written as 103 ones. */
seq_t a(idx_t n)
{
    /* Handle base 1 explicitly – 103 ones */
    if (n == 1) {
        seq_t res = 0;
        /* 103 iterations – safe because 103 is a compile‑time constant */
        for (idx_t i = 0; i < 103; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Transform 103 into base n */
    seq_t val = 103;
    /* 103 < 2^7, so at most 7 digits in base 2;
       use a small fixed array for safety */
    uint8_t digits[16];
    idx_t len = 0;
    while (val > 0) {
        digits[len++] = (uint8_t)(val % n);
        val /= n;
    }

    /* Convert the digit array (reversed) to a decimal number */
    seq_t res = 0;
    for (idx_t i = 0; i < len; ++i) {
        res = res * 10 + (seq_t)digits[len - 1 - i];
    }
    return res;
}
