/*
 * A024999  Expansion of 1/((1-x)(1-8x)(1-10x)(1-11x)).
 * https://oeis.org/A024999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 307*a(n-2) + 1158*a(n-3) - 880*a(n-4), with
 *            a(0)=1, a(1)=30, a(2)=593, a(3)=9738
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)30;
    if (n == 2u)
        return (seq_t)593;
    if (n == 3u)
        return (seq_t)9738;

    seq_t a0 = (seq_t)1;     /* a(n-4)   */
    seq_t a1 = (seq_t)30;    /* a(n-3)   */
    seq_t a2 = (seq_t)593;   /* a(n-2)   */
    seq_t a3 = (seq_t)9738;  /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a4 = 30 * a3 - 307 * a2 + 1158 * a1 - 880 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
