/*
 * A008567  Digits of powers of 6.
 * https://oeis.org/A008567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   a(n) is the n‑th decimal digit of the concatenation 6⁰·6¹·6²·…
 * Range:     u8 n <= 3983, u16 n <= 3983, u32 n <= 3983, u64 n <= 3983,
 *            u128 n <= 3983, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the decimal digits of successive powers of
   6 by repeated multiplication by 6, emitting digits until the required
   index n is reached. It uses a dynamically resizing buffer to hold the
   current power of 6. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Buffer that holds the decimal digits of the current power of 6.
       Digits are stored most‑significant first. */
    size_t cap = 64;
    unsigned char *digits = (unsigned char *)malloc(cap);
    if (!digits) return 0;          /* allocation failed – unlikely */

    size_t len = 1;                 /* number of digits currently stored */
    digits[0] = 1;                  /* 6⁰ = 1 */

    idx_t count = 0;                /* number of digits already emitted */

    for (;;) {
        /* Emit the digits of the current power. */
        for (size_t i = 0; i < len; ++i) {
            if (count == n) {
                unsigned char d = digits[i];
                free(digits);
                return (seq_t)d;
            }
            ++count;
        }

        /* Multiply the current number by 6 to obtain the next power. */
        unsigned int carry = 0;
        for (idx_t i = (idx_t)len - 1; ; --i) {
            unsigned int prod = digits[i] * 6 + carry;
            digits[i] = prod % 10;
            carry = prod / 10;
            if (i == 0) break;
        }

        if (carry > 0) {
            /* Ensure the buffer is large enough. */
            if (len + 1 > cap) {
                cap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(digits, cap);
                if (!tmp) {
                    free(digits);
                    return 0;       /* allocation failed */
                }
                digits = tmp;
            }
            /* Shift digits to make room for the carry at the most
               significant position. */
            for (idx_t i = (idx_t)len; i > 0; --i)
                digits[i] = digits[i - 1];
            digits[0] = (unsigned char)carry;
            ++len;
        }
    }
}
