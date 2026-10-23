/*
 * A041840  Numerators of continued fraction convergents to sqrt(442).
 * https://oeis.org/A041840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 42*a(n-1) + a(n-2), a(0)=21, a(1)=883
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)21;
    if (n == 1) return (seq_t)883;

    seq_t prev2 = (seq_t)21;            /* a(n-2)       */
    seq_t prev1 = (seq_t)883;           /* a(n-1)       */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)42 + prev2;  /* a(i)         */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;                         /* a(n)         */
}
