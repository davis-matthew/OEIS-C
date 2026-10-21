/*
 * A099456  Expansion of 1/(1 - 4*x + 5*x^2).
 * https://oeis.org/A099456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=4, a(n)=4*a(n-1)-5*a(n-2)
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 26, i64 n <= 54, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prevprev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)4;  /* a(1) */
    seq_t cur      = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = ((seq_t)4) * prev - ((seq_t)5) * prevprev;
        prevprev = prev;
        prev     = cur;
    }

    return cur;
}
