/*
 * A051667  a(n) = 6*2^n - 4*n - 6.
 * https://oeis.org/A051667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - 4*n - 6
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2^n as a seq_t in a loop (exact for arbitrary‑precision seq_t,
       and bounded for the 64‑bit build up to the tested range). */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    seq_t res = (seq_t)6 * pow2;    /* 6 * 2^n */
    res -= (seq_t)4 * n;            /* subtract 4*n */
    res -= (seq_t)6;                /* subtract 6 */
    return res;
}
