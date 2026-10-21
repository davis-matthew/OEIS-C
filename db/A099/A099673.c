/*
 * A099673  Partial sums of repdigits of A002280.
 * https://oeis.org/A099673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 21*a(n-2) + 10*a(n-3), with a(1)=6, a(2)=72,
 *            a(3)=738
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == (idx_t)1) return (seq_t)6;
    if (n == (idx_t)2) return (seq_t)72;
    if (n == (idx_t)3) return (seq_t)738;

    /* For n > 3, use the linear recurrence. */
    seq_t a1 = (seq_t)6;     /* a(1)   */
    seq_t a2 = (seq_t)72;    /* a(2)   */
    seq_t a3 = (seq_t)738;   /* a(3)   */
    seq_t an = a3;           /* will hold a(k) during iteration */

    for (idx_t k = (idx_t)4; k <= n; ++k) {
        an = (seq_t)((seq_t)12 * a3 - (seq_t)21 * a2 + (seq_t)10 * a1);
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
