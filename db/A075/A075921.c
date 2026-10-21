/*
 * A075921  Second column of triangle A075502.
 * https://oeis.org/A075921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -7^n + 2*14^n Recurrence: a(0)=1, a(1)=21,
 *            a(n)=21*a(n-1)-98*a(n-2) for n>=2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)21;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)21 * a1 - (seq_t)98 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
