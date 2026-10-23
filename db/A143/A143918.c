/*
 * A143918  G.f. A(x) satisfies: A(x) = 1/(1-x)^2 + x^2*A'(x).
 * https://oeis.org/A143918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (n-1) * a(n-1) + n + 1 for n > 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;            /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* (i-1) * res + (i+1) */
        seq_t term = (seq_t)(i - 1);
        res = term * res + (seq_t)i + 1;
    }
    return res;
}
