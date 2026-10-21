/*
 * A052124  Expansion of e.g.f. exp(-2*x)/(1-x)^3.
 * https://oeis.org/A052124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 2*(n-1)*a(n-2); a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;

    seq_t a0 = 1;          /* a(n-2) */
    seq_t a1 = 1;          /* a(n-1) */
    seq_t an = 1;          /* current term */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* an = i*a1 + 2*(i-1)*a0;  all values are of type seq_t */
        an = (seq_t)i * a1 + (seq_t)2 * (seq_t)(i - 1) * a0;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
