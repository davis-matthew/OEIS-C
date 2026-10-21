/*
 * A052713  Expansion of e.g.f. (1-sqrt(1-8*x))/2.
 * https://oeis.org/A052713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 2, a(n+1) = 4 * (2*n - 1) * a(n) for n >= 1
 *            (OEIS A052713)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t res = (seq_t)2;          /* a(1) */
    for (idx_t i = 1; i < n; ++i)  /* compute a(2), a(3), …, a(n) */
    {
        res *= (seq_t)(4 * (2 * i - 1));
    }
    return res;
}
