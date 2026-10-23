/*
 * A014145  Partial sums of A007489.
 * https://oeis.org/A014145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*(n+1-k)!. Recurrence: a(n) = (n+2)*a(n-1)
 *            -(2*n+1)*a(n-2) + n*a(n-3), n>2, with initial values a(0)=0,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)0;      /* a(n-3) */
    seq_t a1 = (seq_t)1;      /* a(n-2) */
    seq_t a2 = (seq_t)4;      /* a(n-1) */
    seq_t an = (seq_t)0;

    idx_t i;
    for (i = 3; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        an = (i_seq + 2) * a2 - ((i_seq * 2) + 1) * a1 + i_seq * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
