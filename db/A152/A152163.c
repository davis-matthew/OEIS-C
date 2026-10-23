/*
 * A152163  a(n) = a(n-1)+a(n-2), n>1 ; a(0)=1, a(1)=-1.
 * https://oeis.org/A152163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)+a(n-2), a(0) = 1, a(1) = -1
 * Range:     i8 n <= 13, i16 n <= 25, i32 n <= 48, i64 n <= 94, i128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)-1;

    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)-1;     /* a(1) */
    seq_t curr;

    for (idx_t i = 2u; i <= n; ++i) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
