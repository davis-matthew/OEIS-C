/*
 * A212542  Base 2i representation of negative integers.
 * https://oeis.org/A212542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 204, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base 2i representation of negative integers.
   The value is obtained by converting -n to base –4 (digits 0–3),
   then writing those digits in decimal with a zero inserted
   between each pair to account for the missing odd powers
   of 2i. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the smallest n; the sequence starts at n = 1. */
    if (n == 0) {
        return 0;
    }

    /* Work in signed 64‑bit arithmetic; n is small in the test data
       (≤ 10000), so this is safe. */
    int64_t x = -(int64_t)n;

    /* Store base‑–4 digits in least‑significant‑first order. */
    unsigned char digits[64];
    size_t m = 0;

    while (x != 0) {
        int r = (int)(x % -4);      /* remainder may be negative */
        if (r < 0) {
            r += 4;                 /* make it a non‑negative digit */
            x = (x / -4) + 1;       /* adjust quotient accordingly */
        } else {
            x = x / -4;
        }
        digits[m++] = (unsigned char)r;
    }

    /* Build the decimal representation by iterating from
       most significant digit to least, inserting a zero after
       each digit except the last. */
    seq_t result = 0;
    for (ptrdiff_t i = (ptrdiff_t)m - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
        if (i > 0) {
            result *= 10;           /* insert implicit zero */
        }
    }
    return result;
}
