/*
 * A144044  a(n) = ((1+sqrt(11))^n-(1-sqrt(11))^n)^2/44.
 * https://oeis.org/A144044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((1+sqrt(11))^n-(1-sqrt(11))^n)^2/44 Recurrence: a(n) =
 *            14*a(n-1)+140*a(n-2)-1000*a(n-3) Initial values: a(1)=1, a(2)=4,
 *            a(3)=196
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)4;
    if (n == 3u) return (seq_t)196;

    seq_t a1 = (seq_t)1;    // a(n-3)
    seq_t a2 = (seq_t)4;    // a(n-2)
    seq_t a3 = (seq_t)196;  // a(n-1)
    seq_t an;

    for (idx_t i = 4u; i <= n; ++i)
    {
        an = (seq_t)((seq_t)14 * a3 + (seq_t)140 * a2 - (seq_t)1000 * a1);
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
