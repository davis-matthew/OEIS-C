/*
 * A060214  Successive digits of the Lucas sequence.
 * https://oeis.org/A060214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the nth digit of the concatenation of Lucas numbers
 *            starting with L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)
 * Range:     u8 n <= 20, u16 n <= 69, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t a = (seq_t)2;   /* L(0) */
    seq_t b = (seq_t)1;   /* L(1) */
    idx_t digits_before = 0;

    while (1) {
        int len = num_digits(a);                 /* number of decimal digits of a */
        if (digits_before + len > n) {          /* target digit is in this number */
            idx_t pos = n - digits_before;      /* position within the number (0=most signif.) */
            seq_t divisor = pow10((idx_t)(len - 1 - pos));
            seq_t digit = (a / divisor) % (seq_t)10;
            return digit;
        }
        digits_before += (idx_t)len;

        /* advance to next Lucas number */
        seq_t next = a + b;
        a = b;
        b = next;
    }
}
