/*
 * A052776  a(n) = 4!*n*Stirling2(n-1,4).
 * https://oeis.org/A052776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24 * n * Stirling2(n-1, 4) (a(0)=a(1)=a(2)=a(3)=a(4)=0)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Terms 0 through 4 are zero */
    if (n <= 4) return (seq_t)0;

    /* compute S(n-1,4) using the recurrence
       S(n,1)=1 (for n>0), S(n,2)=2*S(n-1,2)+S(n-1,1),
       S(n,3)=3*S(n-1,3)+S(n-1,2),
       S(n,4)=4*S(n-1,4)+S(n-1,3)          */
    seq_t s2 = 0;   /* S(i,2) for current i   */
    seq_t s3 = 0;   /* S(i,3) */
    seq_t s4 = 0;   /* S(i,4) */

    for (idx_t i = 1; i <= n-1; ++i) {
        seq_t ns2 = (seq_t)2 * s2 + ((i >= 2) ? (seq_t)1 : (seq_t)0);
        seq_t ns3 = (seq_t)3 * s3 + s2;
        seq_t ns4 = (seq_t)4 * s4 + s3;
        s2 = ns2;
        s3 = ns3;
        s4 = ns4;
    }

    /* a(n) = 24 * n * s4 */
    seq_t res = (seq_t)n * (seq_t)24 * s4;
    return res;
}
