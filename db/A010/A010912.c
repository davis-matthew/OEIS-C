/*
 * A010912  Pisot sequences E(3,7), P(3,7).
 * https://oeis.org/A010912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=7, a(2)=16; a(n)=3*a(n-1)-2*a(n-2)+a(n-3), n>=3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a simple linear recurrence that matches OEIS A010912. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)16;

    seq_t a0 = (seq_t)3;   /* a(n-3) */
    seq_t a1 = (seq_t)7;   /* a(n-2) */
    seq_t a2 = (seq_t)16;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)3 * a2 - (seq_t)2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
