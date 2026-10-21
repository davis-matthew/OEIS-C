/*
 * A052765  Expansion of e.g.f.: -x^2*(log(1-x))^3.
 * https://oeis.org/A052765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6 * n * (n-1) * |s(n-2,3)|, where s(m,k) is the unsigned
 *            * Stirling number of the first kind. For n<5 the value is 0. The
 *            recurrence * uses s(i,1)=i! , s(i,2) = s(i-1,1)+(i-1)*s(i-1,2)
 *            and * s(i,3) = s(i-1,2)+(i-1)*s(i-1,3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices do not contribute to the sequence. */
    if (n < 5)
        return (seq_t)0;

    /* We need s(n-2,3).  Compute it iteratively up to m=n-2. */
    idx_t m = n - 2;           /* m ≥ 3 for n ≥ 5 */
    seq_t s1 = (seq_t)1;       /* s(i,1) = (i-1)!  when i starts at 1 */
    seq_t s2 = (seq_t)0;       /* s(i,2) for current i  (starts 0 at i=1) */
    seq_t s3 = (seq_t)0;       /* s(i,3) for current i  (starts 0 at i=1) */

    /* Advance i from 2 up to m inclusive. */
    for (idx_t i = 2; i <= m; ++i) {
        seq_t mult = (seq_t)(i - 1);            /* (i-1) factor */
        seq_t ns1 = mult * s1;                 /* new s(i,1) */
        seq_t ns2 = s1 + mult * s2;            /* new s(i,2) */
        seq_t ns3 = s2 + mult * s3;            /* new s(i,3) */
        s1 = ns1;
        s2 = ns2;
        s3 = ns3;
    }

    /* Final result: a(n) = 6 * n * (n-1) * s3. */
    seq_t prod = (seq_t)6 * (seq_t)n * (seq_t)(n - 1);
    return prod * s3;
}
