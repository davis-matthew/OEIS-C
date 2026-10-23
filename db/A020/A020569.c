/*
 * A020569  Expansion of g.f. 1/((1 - 5*x)*(1 - 11*x)*(1 - 12*x)).
 * https://oeis.org/A020569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 247*a(n-2) + 660*a(n-3) with a(0)=1, a(1)=28,
 *            a(2)=537
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)537;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)28;
    seq_t a2 = (seq_t)537;
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 * (seq_t)28 - a1 * (seq_t)247 + a0 * (seq_t)660;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
