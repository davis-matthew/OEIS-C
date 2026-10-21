/*
 * A099513  Row sums of triangle A099512, so that a(n) = Sum_{k=0..n}
 *          coefficient of z^k in (1 + 3*z + z^2)^(n-[k/2]), where [k/2] is
 *          the integer floor of k/2.
 * https://oeis.org/A099513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 7*a(n-3) - a(n-4) (offset 0) Initial
 *            values: a(0)=1, a(1)=4, a(2)=8, a(3)=27
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)27;

    /* Use a sliding window of the last four values */
    seq_t a0 = (seq_t)1;      /* a(n-4)   */
    seq_t a1 = (seq_t)4;      /* a(n-3)   */
    seq_t a2 = (seq_t)8;      /* a(n-2)   */
    seq_t a3 = (seq_t)27;     /* a(n-1)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 2 * a3 + a2 + 7 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
