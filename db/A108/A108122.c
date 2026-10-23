/*
 * A108122  G.f.: (1-2*x^2)/(1-x-2*x^2-x^3).
 * https://oeis.org/A108122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3); a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    seq_t a0 = (seq_t)1, a1 = (seq_t)1, a2 = (seq_t)1, an;
    for (idx_t i = 3; i <= n; ++i)
    {
        an = a2 + (seq_t)2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
