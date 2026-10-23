/*
 * A028090  Expansion of 1/((1-3x)(1-7x)(1-8x)(1-10x)).
 * https://oeis.org/A028090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(n+4)-42*8^(n+3)+5*7^(n+4)-3^(n+4))/420 Equivalent
 *            recurrence: a(n) = 28*a(n-1) - 281*a(n-2) + 1178*a(n-3) -
 *            1680*a(n-4) with a(0)=1, a(1)=28, a(2)=503, a(3)=7394
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)503;
    if (n == 3) return (seq_t)7394;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)28;
    seq_t a2 = (seq_t)503;
    seq_t a3 = (seq_t)7394;
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = a3 * (seq_t)28 + a1 * (seq_t)1178;
        seq_t t2 = a2 * (seq_t)281 + a0 * (seq_t)1680;
        an = t1 - t2;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
