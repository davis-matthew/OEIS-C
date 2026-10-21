/*
 * A051958  a(n) = 2*a(n-1) + 24*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A051958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 24*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev  = (seq_t)1;  /* a[1] */
    seq_t prev2 = (seq_t)0;  /* a[0] */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = ((seq_t)2) * prev + ((seq_t)24) * prev2;
        prev2 = prev;
        prev  = next;
    }
    return prev;  /* a[n] */
}
