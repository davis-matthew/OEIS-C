/*
 * A153318  Numerators of continued fraction convergents to sqrt(6/5).
 * https://oeis.org/A153318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, for n>=2 if n is even: a(n)=2*a(n-1)+a(n-2) if
 *            n is odd : a(n)=10*a(n-1)+a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)11;  /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (i & 1)                     /* odd index: a(i)=10*a(i-1)+a(i-2) */
            curr = (seq_t)10 * prev1 + prev2;
        else                           /* even index: a(i)=2*a(i-1)+a(i-2) */
            curr = (seq_t)2 * prev1 + prev2;

        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
