/*
 * A043291  Every run length in base 2 is 2.
 * https://oeis.org/A043291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(4^(n+1)/5) Recurrence: a(1)=3, a(2)=12, a(3)=51,
 *            a(n)=4*a(n-1)+a(n-2)-4*a(n-3) for n>3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1; return 0 as placeholder */
        return (seq_t)0;

    if (n == 1)          return (seq_t)3;
    if (n == 2)          return (seq_t)12;
    if (n == 3)          return (seq_t)51;

    seq_t a_3 = (seq_t)51;  /* a(n-1) */
    seq_t a_2 = (seq_t)12;  /* a(n-2) */
    seq_t a_1 = (seq_t)3;   /* a(n-3) */
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i)
    {
        a_n = (seq_t)4 * a_3 + a_2 - (seq_t)4 * a_1;
        a_1 = a_2;
        a_2 = a_3;
        a_3 = a_n;
    }
    return a_3;
}
