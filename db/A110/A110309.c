/*
 * A110309  Expansion of (1+3*x+x^2)/((1+x+x^2)*(1+5*x+x^2)).
 * https://oeis.org/A110309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-3, a(2)=12, a(3)=-57, a(n) = -6*a(n-1) -7*a(n-2)
 *            -6*a(n-3) - a(n-4) for n>3
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 28, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)-57;

    seq_t a0 = (seq_t)1;   // a(n-4)
    seq_t a1 = (seq_t)-3;  // a(n-3)
    seq_t a2 = (seq_t)12;  // a(n-2)
    seq_t a3 = (seq_t)-57; // a(n-1)
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = a3 * (seq_t)-6 + a2 * (seq_t)-7 + a1 * (seq_t)-6 + a0 * (seq_t)-1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
