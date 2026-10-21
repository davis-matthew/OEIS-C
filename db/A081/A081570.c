/*
 * A081570  Fifth binomial transform of F(n+1).
 * https://oeis.org/A081570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=6, a(n)=11*a(n-1)-29*a(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)6;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)6;   /* a(n-1) */
    seq_t an  = 0;         /* a(n) */

    for (idx_t i = 2U; i <= n; ++i) {
        an = 11 * a1 - 29 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
