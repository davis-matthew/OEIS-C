/*
 * A097163  Expansion of (1+x-x^2)/((1-x)*(1-4*x^2)).
 * https://oeis.org/A097163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) (a(0)=1, a(1)=2, a(2)=5)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly for speed */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)5;

    /* Use recurrence a(n) = a(n-1)+4*a(n-2)-4*a(n-3) */
    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)2;  /* a(n-2) */
    seq_t a2 = (seq_t)5;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = a2 + (seq_t)4 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2;
}
