/*
 * A155466  a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) for n > 2; a(0) = 28, a(1) =
 *          207, a(2) = 1248.
 * https://oeis.org/A155466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) with a(0)=28, a(1)=207,
 *            a(2)=1248
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)28;
    if (n == 1) return (seq_t)207;
    if (n == 2) return (seq_t)1248;

    seq_t a0 = (seq_t)28;
    seq_t a1 = (seq_t)207;
    seq_t a2 = (seq_t)1248;
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        an = ((seq_t)7 * a2) - ((seq_t)7 * a1) + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
