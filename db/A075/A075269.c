/*
 * A075269  Product of Lucas numbers and inverted Lucas numbers:
 *          a(n)=A000032(n)*A075193(n).
 * https://oeis.org/A075269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -3*a(n-1) - a(n-2) + 5, with a(0)=2, a(1)=-3
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)(-3);

    seq_t prev2 = (seq_t)2;       /* a(n-2) */
    seq_t prev1 = (seq_t)(-3);    /* a(n-1) */
    seq_t cur = prev1;            /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = ((seq_t)(-3)) * prev1 - prev2 + (seq_t)5;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
