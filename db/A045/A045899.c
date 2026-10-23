/*
 * A045899  Numbers k such that k+1 and 3*k+1 are perfect squares.
 * https://oeis.org/A045899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 8 with a(1)=0, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n <= 1) return (seq_t)0;
    if (n == 2) return (seq_t)8;

    seq_t prev = (seq_t)0;   /* a(1) */
    seq_t cur  = (seq_t)8;   /* a(2) */
    seq_t nxt;

    for (idx_t i = 3; i <= n; ++i) {
        nxt = mul_small(cur, 14); /* 14 * a(n-1) */
        nxt -= prev;              /* subtract a(n-2) */
        nxt += (seq_t)8;           /* add constant 8 */
        prev = cur;
        cur = nxt;
    }
    return cur;
}
