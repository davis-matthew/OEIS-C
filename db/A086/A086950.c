/*
 * A086950  Binomial transform of decagonal numbers A001107.
 * https://oeis.org/A086950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*2^n*(2*n-1)/2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=12, a(n)=6*a(n-1)-12*a(n-2)+8*a(n-3) */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)12;
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        ai = (seq_t)6 * a2 - (seq_t)12 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
