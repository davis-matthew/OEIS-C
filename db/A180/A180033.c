/*
 * A180033  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1 + x)/(1 - 5*x - 5*x^2).
 * https://oeis.org/A180033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The type seq_t is supplied by the build (uint8_t … uint128_t, or a big‑int class). */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)6;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = 5 * a1 + 5 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
