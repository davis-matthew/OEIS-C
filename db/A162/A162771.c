/*
 * A162771  a(n) = ((2+sqrt(5))*(3+sqrt(5))^n + (2-sqrt(5))*(3-sqrt(5))^n)/2.
 * https://oeis.org/A162771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) for n > 1; a(0)=2, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)11;

    seq_t a0 = (seq_t)2;   /* a(0) */
    seq_t a1 = (seq_t)11;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a2 = 6 * a1 - 4 * a0;  /* a(i) = 6*a(i-1) - 4*a(i-2) */
        a0 = a1;
        a1 = a2;
    }
    return a1;    /* a(n) */
}
