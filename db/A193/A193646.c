/*
 * A193646  Number of arrays of -6..6 integers x(1..n) with every x(i) in a
 *          subsequence of length 1 or 2 with sum zero.
 * https://oeis.org/A193646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +10*a(n-2) + a(n-3) for n>=4 Initial values:
 *            a(1)=1, a(2)=13, a(3)=37
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)37;

    seq_t a1 = (seq_t)1;  /* a(n-3)  */
    seq_t a2 = (seq_t)13; /* a(n-2)  */
    seq_t a3 = (seq_t)37; /* a(n-1)  */
    seq_t an = a3;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)2 * a3 + (seq_t)10 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
