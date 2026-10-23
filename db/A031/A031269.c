/*
 * A031269  Write the (n+1)st Fibonacci number in base 5 and juxtapose.
 * https://oeis.org/A031269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth base‑5 digit of the concatenated string of *
 *            base‑5 representations of Fibonacci numbers starting with F2
 * Range:     u8 n <= 28, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* out of offset, undefined */
    seq_t f_prev = 1;   /* F1 */
    seq_t f_cur  = 1;   /* F2 – first output */
    idx_t pos = 0;      /* digits counted so far (0‑based) */

    while (true) {
        /* obtain base‑5 digits of f_cur in reverse order */
        seq_t tmp = f_cur;
        int digits[200];
        int len = 0;
        while (tmp != 0) {
            digits[len++] = (int)(tmp % 5);
            tmp /= 5;
        }
        if (len == 0) {          /* shouldn't happen for Fibonacci >=1 */
            digits[0] = 0;
            len = 1;
        }

        /* Does the desired position lie within this number? */
        if (n <= pos + len) {
            idx_t idx = n - pos - 1;                /* 0‑based within this number */
            int digit = digits[len - 1 - idx];      /* most significant first */
            return (seq_t)digit;
        }

        /* Move to next Fibonacci number */
        pos += len;
        seq_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur = f_next;
    }
}
