/*
 * A052564  Expansion of e.g.f. x*(1-x)/(1-2*x).
 * https://oeis.org/A052564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n-1) for n >= 3, a(0) = 0, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A052564: a(n) = 2^(n-2) * n! for n > 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    seq_t res = (seq_t)2;   /* a(2) */
    for (idx_t k = 3; k <= n; ++k)
    {
        res = res * ((seq_t)2 * (seq_t)k);
    }
    return res;
}
