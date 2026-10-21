/*
 * A051792  a(n) = (-1)^(n-1)*(a(n-1) - a(n-2)), a(1)=1, a(2)=1.
 * https://oeis.org/A051792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n-1) * (a(n-1) - a(n-2)), a(1)=1, a(2)=1
 * Range:     i8 n <= 26, i16 n <= 50, i32 n <= 96, i64 n <= 188,
 *            i128 n <= 372, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2) return (seq_t)1;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2)   */
    seq_t a_nm1 = (seq_t)1;   /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t diff = a_nm1 - a_nm2;
        if (i & 1ULL)       /* n odd → sign +1 */
            cur = diff;
        else                /* n even → sign -1 */
            cur = -diff;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
