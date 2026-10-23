/*
 * A163166  a(n) = 20*a(n-1)-95*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A163166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1)-95*a(n-2) for n>1; a(0)=1; a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)10;
    seq_t an;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = (seq_t)20 * a1 - (seq_t)95 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
