/*
 * A077855  Expansion of 1/((1-2*x+x^2-x^3)*(1-x)).
 * https://oeis.org/A077855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + 1, with a(0)=1, a(1)=3,
 *            a(2)=6, a(3)=11
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 77, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)11;

    seq_t a0 = (seq_t)1,   /* a(n-3) */
          a1 = (seq_t)3,   /* a(n-2) */
          a2 = (seq_t)6,   /* a(n-1) */
          a3 = (seq_t)11;  /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = 2 * a3 - a2 + a1 + 1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
