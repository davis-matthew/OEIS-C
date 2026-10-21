/*
 * A052546  Expansion of (1-x)/(1-x-x^2-2*x^3+2*x^4).
 * https://oeis.org/A052546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) - 2*a(n-4) with a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=3
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 75, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* t0 = a(n-4), t1 = a(n-3), t2 = a(n-2), t3 = a(n-1) */
    seq_t t0 = (seq_t)1;   /* a(0) */
    seq_t t1 = (seq_t)0;   /* a(1) */
    seq_t t2 = (seq_t)1;   /* a(2) */
    seq_t t3 = (seq_t)3;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = t3 + t2 + (t1 << 1) - (t0 << 1); /* 2*t1 and 2*t0 using shift */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = val;
    }
    return t3;
}
