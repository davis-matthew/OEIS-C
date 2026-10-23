/*
 * A164035  a(n) = ((4+3*sqrt(2))*(5+sqrt(2))^n +
 *          (4-3*sqrt(2))*(5-sqrt(2))^n)/4.
 * https://oeis.org/A164035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2); a(0) = 2, a(1) = 13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)13;

    seq_t a0 = (seq_t)2;   /* a(0) */
    seq_t a1 = (seq_t)13;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = a1 * (seq_t)10;   /* 10*a(n-1) */
        temp -= a0 * (seq_t)23;        /* -23*a(n-2) */
        a0 = a1;                       /* shift for next iteration */
        a1 = temp;
    }
    return a1;
}
