/*
 * A128535  a(n) = F(n)*L(n-2) where F = Fibonacci and L = Lucas numbers.
 * https://oeis.org/A128535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3); Initial values: a(0)=0,
 *            a(1)=-1, a(2)=2
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 24, i64 n <= 47, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)2;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)-1;  /* a(n-2) */
    seq_t a2 = (seq_t)2;   /* a(n-1) */

    seq_t an;
    for (idx_t i = 3; i <= n; ++i) {
        an = (a2 + a2) + (a1 + a1) - a0;  /* 2*a2 + 2*a1 - a0 */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
