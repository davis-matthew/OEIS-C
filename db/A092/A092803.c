/*
 * A092803  Expansion of (1-5*x)/((1-2*x)*(1-6*x)).
 * https://oeis.org/A092803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 12*a(n-2), a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a1;
        tmp *= (seq_t)8;          /* 8*a(n-1) */
        seq_t sub = a0;
        sub *= (seq_t)12;         /* 12*a(n-2) */
        seq_t a2 = tmp - sub;     /* a(n) = 8*a(n-1) - 12*a(n-2) */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
