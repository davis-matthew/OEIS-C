/*
 * A234789  Number of (n+1) X (1+1) 0..3 arrays with each 2 X 2 subblock
 *          having the number of clockwise edge increases less than or equal
 *          to the number of counterclockwise edge increases.
 * https://oeis.org/A234789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 36*a(n-2) + 20*a(n-3) (n>=4) a(1)=204,
 *            a(2)=2504, a(3)=30536
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined offset, safe fallback */
    if (n == 1) return (seq_t)204;
    if (n == 2) return (seq_t)2504;
    if (n == 3) return (seq_t)30536;

    seq_t a1 = (seq_t)204;
    seq_t a2 = (seq_t)2504;
    seq_t a3 = (seq_t)30536;
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 15 * a3 - 36 * a2 + 20 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
