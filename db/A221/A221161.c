/*
 * A221161  G.f.: Sum_{n>=0} (4*n+3)^n * x^n / (1 + (4*n+3)*x)^n.
 * https://oeis.org/A221161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+5) * 4^(n-1) * n! for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t pow4 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow4 *= 4;

    seq_t res = (seq_t)(2 * n + 5);
    res *= pow4;
    res *= fact;
    return res;
}
