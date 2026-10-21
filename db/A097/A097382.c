/*
 * A097382  a(h) = d(h,j) = lcm( f(h,j,1) ... f(h,j,h) ), when j=2.
 * https://oeis.org/A097382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( f(n,2,1), f(n,2,2), …, f(n,2,n) ) where f(h,2,a) =
 *            (3h – 2a) / ( gcd(a,h) * gcd(3, a/gcd(a,h)) )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes lcm of two seq_t numbers */
static seq_t lcm_seq(seq_t x, seq_t y)
{
    seq_t g = gcd(x, y);
    /* prevent overflow: x/g * y */
    return (x / g) * y;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t L = 1;                     /* running lcm */
    seq_t hn = (seq_t)n;              /* h as seq_t */
    const seq_t three = (seq_t)3;
    const seq_t two   = (seq_t)2;

    for (idx_t ai = 1; ai <= n; ++ai)
    {
        seq_t a_val = (seq_t)ai;
        seq_t d = gcd(a_val, hn);          /* gcd(a,h) */
        seq_t m = a_val / d;               /* a/gcd(a,h) */
        seq_t g = gcd(three, m);           /* gcd(3, a/gcd(a,h)) */

        seq_t numerator   = three * hn - two * a_val;  /* 3h – 2a */
        seq_t denominator = d * g;                    /* gcd(a,h) * gcd(3,m) */

        seq_t f_val = numerator / denominator;       /* f(h,2,a) */

        L = lcm_seq(L, f_val);
    }
    return L;
}
