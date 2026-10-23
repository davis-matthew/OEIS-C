/*
 * A185963  Row sums of number triangle A185962.
 * https://oeis.org/A185963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + a(n-3) with initial values a(0)=1,
 *            a(1)=0, a(2)=-2
 * Range:     i8 n <= 11, i16 n <= 25, i32 n <= 51, i64 n <= 105,
 *            i128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return -(seq_t)2;

    seq_t a0 = (seq_t)1;      /* a(i-3)  */
    seq_t a1 = (seq_t)0;      /* a(i-2)  */
    seq_t a2 = -(seq_t)2;     /* a(i-1)  */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = ((seq_t)2 * a2) - ((seq_t)3 * a1) + a0;  /* a(i)   */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
