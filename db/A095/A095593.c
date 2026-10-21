/*
 * A095593  a(n) = 101 written in base 12 - n.
 * https://oeis.org/A095593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of the decimal number 101 in base (12-n),
 *            interpreted as a decimal integer
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n).  Base B = 12 - n.
   If B > 1:  standard base conversion for positive B.
   If B == 1:  unary representation (101 ones).
   If B <= 0 or B == -1: return 0 (undefined values). */
seq_t a(idx_t n)
{
    /* Compute base as signed 128‑bit to avoid overflow when n > 2^63. */
    __int128 base128 = (__int128)12 - (__int128)n;

    /* Base 0 or -1: undefined, return 0. */
    if (base128 == 0 || base128 == -1)
        return 0;

    /* Unary base (B == 1): 101 ones. */
    if (base128 == 1) {
        seq_t res = 0;
        for (int i = 0; i < 101; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* General case for B != 1. */
    /* Prepare an array for digits (most significant digit first).  Max digits is tiny. */
    int digits[256];
    int cnt = 0;

    /* Number to represent is 101 decimal. */
    __int128 cur = 101;

    if (base128 > 1) {                 /* Positive base */
        __int128 B = base128;
        while (cur > 0) {
            int r = (int)(cur % B);    /* r < B, fits in int */
            digits[cnt++] = r;
            cur /= B;
        }
    } else {                           /* Negative base (base128 < -1) */
        __int128 B = -base128;          /* |B| */
        while (cur != 0) {
            int r = (int)(cur % B);
            if (r < 0) r += (int)B;     /* ensure remainder non‑negative */
            digits[cnt++] = r;
            cur = (cur - r) / base128; /* base128 is negative */
        }
    }

    /* Convert digit array to decimal integer represented by those digits. */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i)
        res = res * 10 + (seq_t)digits[i];

    return res;
}
