/*
 * A106666  Expansion of g.f. (1-4*x^2+2*x^3)/((1-x)*(1-2*x-2*x^2+2*x^3)).
 * https://oeis.org/A106666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=5, a(3)=13; For n>=4: a(n) = 3*a(n-1) -
 *            4*a(n-3) + 2*a(n-4) (derived from the denominator
 *            (1-x)(1-2x-2x^2+2x^3) of the generating function)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)13;

    /* Ring buffer holding a[n-4], a[n-3], a[n-2], a[n-1] */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)3;   /* a(1) */
    buf[2] = (seq_t)5;   /* a(2) */
    buf[3] = (seq_t)13;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Indices into the ring buffer */
        idx_t idx_m1 = (i - 1) & 3;   /* a[i-1] */
        idx_t idx_m3 = (i - 3) & 3;   /* a[i-3] */
        idx_t idx_m4 = (i - 4) & 3;   /* a[i-4] */

        seq_t term = buf[idx_m1] * (seq_t)3;          /* 3*a[i-1]   */
        term = term + (buf[idx_m4] * (seq_t)2);       /* +2*a[i-4]  */
        term = term - (buf[idx_m3] * (seq_t)4);       /* -4*a[i-3]  */

        buf[i & 3] = term;        /* store a[i] */
    }
    return buf[n & 3];
}
