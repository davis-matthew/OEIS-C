/*
 * A042200  Numerators of continued fraction convergents to sqrt(626).
 * https://oeis.org/A042200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=25, a(1)=1251, a(n) = 50*a(n-1)+a(n-2) for n>1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)25;
    if (n == 1) return (seq_t)1251;

    seq_t prevprev = (seq_t)25;
    seq_t prev = (seq_t)1251;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)50 * prev + prevprev;
        prevprev = prev;
        prev = cur;
    }

    return prev;
}
