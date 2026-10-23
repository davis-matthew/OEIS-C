/*
 * A122910  Expansion of (1-2x-3x^2)/((1-2x)(1+4x)(1-8x)).
 * https://oeis.org/A122910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=6*a(n-1)+24*a(n-2)-64*a(n-3) for n>=3 Initial terms:
 *            a(0)=1, a(1)=4, a(2)=45
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)45;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)45;  /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term = a2 * (seq_t)6 + a1 * (seq_t)24;
        an = term - a0 * (seq_t)64;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
