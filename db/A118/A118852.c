/*
 * A118852  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 8.
 * https://oeis.org/A118852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(n)=8+rotateLeft(a(n-1)).
   rotateLeft(x) moves the most significant decimal digit of x
   to the end of its decimal representation. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len.h"     /* digit_len(seq_t) */
#include "../utils/pow10.h"        /* pow10(idx_t) */

seq_t a(idx_t n)
{
    /* a(1) is defined as the first term of the sequence. */
    if (n == 0) return 0;          /* behaviour below the offset not required */
    seq_t x = 1;                    /* a(1) */
    if (n == 1) return x;

    /* iterate from n=2 up to the requested n */
    for (idx_t i = 2; i <= n; ++i) {
        int len = digit_len(x);            /* number of decimal digits */
        idx_t e = (idx_t)len - 1;
        seq_t p = pow10(e);                /* 10^(len-1) */
        seq_t d = x / p;                   /* most significant digit */
        seq_t y = x % p;                   /* remaining digits */
        x = y * 10 + d;                    /* rotate left */
        x += 8;                            /* add 8 */
    }
    return x;
}
