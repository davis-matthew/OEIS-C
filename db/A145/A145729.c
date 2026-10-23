/*
 * A145729  Partial sums of A052379.
 * https://oeis.org/A145729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2) + 8*a(n-3) with initial values
 *            a(0)=0, a(1)=8, a(2)=80
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)8;
    if (n == 2u) return (seq_t)80;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)8;
    seq_t a2 = (seq_t)80;
    seq_t an = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        an = (seq_t)10 * a2 - (seq_t)17 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
