/*
 * A176981  Expansion of 2+(1-2*x)/(-1+2*x+x^2).
 * https://oeis.org/A176981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=-1; for n>=3, a(n)=2*a(n-1)+a(n-2)
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 26, i64 n <= 51, i128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)(-1);

    seq_t prev2 = (seq_t)0;        /* a(1) */
    seq_t prev1 = (seq_t)(-1);     /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (prev1 * (seq_t)2) + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
