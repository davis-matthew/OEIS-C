/*
 * A075193  Expansion of (1-2*x)/(1+x-x^2).
 * https://oeis.org/A075193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1)+a(n-2), a(0)=1, a(1)=-3
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 181,
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
        return (seq_t)-3;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-3;  /* a(1) */
    seq_t an = a0;        /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        an = -a1 + a0;    /* a(i) = -a(i-1) + a(i-2) */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
