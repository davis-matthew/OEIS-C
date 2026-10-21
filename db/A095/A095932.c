/*
 * A095932  Number of walks of length 2n+1 between two nodes at distance 3 in
 *          the cycle graph C_10.
 * https://oeis.org/A095932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 13*a(n-2) + 4*a(n-3); a(1)=1, a(2)=5, a(3)=22
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)22;

    seq_t a0 = (seq_t)1,     /* a(n-3) */
          a1 = (seq_t)5,     /* a(n-2) */
          a2 = (seq_t)22;    /* a(n-1) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = 7 * a2 - 13 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
