/*
 * A100230  Main diagonal of triangle A100229.
 * https://oeis.org/A100230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; for n>1, a(n)=3*a(n-1)+a(n-2)+3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t prev2 = (seq_t)1;      /* a(n-2)  */
    seq_t prev1 = (seq_t)2;      /* a(n-1)  */
    seq_t cur;                   /* a(n)    */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        cur = (seq_t)3 * prev1 + prev2 + (seq_t)3;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
