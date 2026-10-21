/*
 * A084850  a(n) = 2^(n-1)*(n^2+2n+2).
 * https://oeis.org/A084850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1) * (n^2 + 2n + 2), offset 0, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;                      /* a(0) = 1 */

    /* compute 2^(n-1) safely, using seq_t shift */
    seq_t power = ((seq_t)1) << (n - 1);      /* n-1 is safe: n>0 */

    /* compute n^2 + 2n + 2 */
    seq_t nn = (seq_t)n;
    seq_t factor = nn * nn + (seq_t)2 * nn + (seq_t)2;

    return power * factor;
}
