/*
 * A027947  a(n) = Sum_{k=m..n} T(k,n-k), where m = floor((n+1)/2); a(n) is
 *          the n-th diagonal-sum of left justified array T given by A027935.
 * https://oeis.org/A027947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = 2*a(n-2) + a(n-3) - a(n-4) for n >= 4 with
 *            initial values a(0)=1, a(1)=1, a(2)=2, a(3)=3. This follows from
 *            the generating function (1 + x)/(1 - 2*x^2 - x^3 + x^4)
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 55, u64 n <= 111,
 *            u128 n <= 222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* maintain the last four computed values:
       v0 = a(n-4), v1 = a(n-3), v2 = a(n-2), v3 = a(n-1) */
    seq_t v0 = (seq_t)1;   /* a0  */
    seq_t v1 = (seq_t)1;   /* a1  */
    seq_t v2 = (seq_t)2;   /* a2  */
    seq_t v3 = (seq_t)3;   /* a3  */
    seq_t vi = (seq_t)0;   /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        vi = (seq_t)2 * v2 + v1 - v0;   /* a(i) = 2*a(i-2)+a(i-3)-a(i-4) */
        /* shift the window for the next iteration */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = vi;
    }
    return vi;
}
