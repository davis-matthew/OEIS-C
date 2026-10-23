/*
 * A102091  Number of perfect matchings in the C_{2n} X P_3 graph (C_{2n} is
 *          the cycle graph on 2n vertices and P_3 is the path graph on 3
 *          vertices).
 * https://oeis.org/A102091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3), a(1)=12, a(2)=32, a(3)=108
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for 0, return 0 safely */
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)32;
    if (n == 3) return (seq_t)108;

    seq_t a1 = (seq_t)12;   /* a(k-3)  */
    seq_t a2 = (seq_t)32;   /* a(k-2)  */
    seq_t a3 = (seq_t)108;  /* a(k-1)  */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 5 * a3 - 5 * a2 + a1;   /* a(i) = 5*a(i-1) - 5*a(i-2) + a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
