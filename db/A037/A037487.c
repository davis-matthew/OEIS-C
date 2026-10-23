/*
 * A037487  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,2.
 * https://oeis.org/A037487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1)+a(n-2)-10*a(n-3), a(1)=1, a(2)=12, a(3)=121
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)121;

    seq_t a1 = (seq_t)1;    /* a(n-3) */
    seq_t a2 = (seq_t)12;   /* a(n-2) */
    seq_t a3 = (seq_t)121;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = a3 * (seq_t)10 + a2 - a1 * (seq_t)10;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
