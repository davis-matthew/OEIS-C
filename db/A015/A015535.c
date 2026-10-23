/*
 * A015535  Expansion of x/(1 - 5*x - 2*x^2).
 * https://oeis.org/A015535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)5 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
