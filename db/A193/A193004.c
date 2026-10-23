/*
 * A193004  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-5*a(n-2)+a(n-3)+2*a(n-4)-a(n-5).
 *            a(0)=1,a(1)=1,a(2)=9,a(3)=29,a(4)=75
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)9, (seq_t)29, (seq_t)75 };

    if (n < 5)
        return init[(size_t)n];

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t ai;

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t pos = (seq_t)4 * a4 + a2 + (seq_t)2 * a1;
        seq_t neg = (seq_t)5 * a3 + a0;
        ai   = pos - neg;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
