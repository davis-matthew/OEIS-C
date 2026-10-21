/*
 * A097310  Chebyshev T-polynomials T(n,14) with Diophantine property.
 * https://oeis.org/A097310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 28*a(n-1) - a(n-2); a(0)=1, a(1)=14 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    seq_t a0 = (seq_t)1;    /* a(n-2) */
    seq_t a1 = (seq_t)14;   /* a(n-1) */
    seq_t an = a1;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = (seq_t)28 * a1 - a0;  /* a(n) = 28*a(n-1) - a(n-2) */
        a0 = a1;
        a1 = an;
    }
    return an;
}
