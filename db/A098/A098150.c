/*
 * A098150  a(n) = 2(a(n-2) - a(n-1)) + a(n-3) where a(0)=-3, a(1)=11 &
 *          a(2)=-30.
 * https://oeis.org/A098150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = -3, a(1) = 11, a(n) = -3*a(n-1) - a(n-2) for n ≥ 2
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 20, i64 n <= 43, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-3;
    if (n == 1)
        return (seq_t)11;

    seq_t a0 = (seq_t)-3;       /* a(n-2) */
    seq_t a1 = (seq_t)11;       /* a(n-1) */
    seq_t a2;                   /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        a2 = (seq_t)-3 * a1 - a0;
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
