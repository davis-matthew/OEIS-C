/*
 * A082134  Expansion of e.g.f. x*exp(3*x)*cosh(x).
 * https://oeis.org/A082134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n*(2^(n-1)+4^(n-1))/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t p2 = 1;   /* 2^(n-1) */
    seq_t p4 = 1;   /* 4^(n-1) */
    for (idx_t i = 1; i < n; ++i) {
        p2 *= (seq_t)2;
        p4 *= (seq_t)4;
    }

    seq_t sum = p2 + p4;       /* 2^(n-1) + 4^(n-1) */
    sum /= (seq_t)2;           /* divide early to avoid overflow */

    return sum * (seq_t)n;     /* n * sum/2 */
}
