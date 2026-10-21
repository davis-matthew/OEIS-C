/*
 * A094941  a(n) is n! times the coefficient of Pi^floor(n/2) in the volume of
 *          an n-dimensional unit ball.
 * https://oeis.org/A094941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-2) * a(n-2) for n > 1, a(0) = 1, a(1) = 2. (Closed
 *            forms: a(2k) = (2k)!/k!, a(2k+1) = 2^(2k+1)*k!)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prevPrev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)2;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)(2 * i - 2) * prevPrev;
        prevPrev = prev;
        prev = cur;
    }
    return prev;
}
