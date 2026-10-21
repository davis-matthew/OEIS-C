/*
 * A073774  Number of plane binary trees of size n+3 and height n.
 * https://oeis.org/A073774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n<3, a(3)=4, a(n) = 1/12 * 2^(n-1) * (2*n^3 + 9*n^2
 *            - 23*n - 78) for n>=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices */
    if (n < 3) return 0;
    if (n == 3) return 4;

    /* Compute polynomial part: 2*n^3 + 9*n^2 - 23*n - 78 */
    seq_t t  = (seq_t)n;          /* cast index to seq_t */
    seq_t t2 = t * t;
    seq_t t3 = t2 * t;
    seq_t poly = (seq_t)2 * t3
                + (seq_t)9 * t2
                - (seq_t)23 * t
                - (seq_t)78;

    /* Compute 2^(n-1) by successive doubling – works for all build types */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i)   /* multiply by 2, n-1 times */
        pow2 *= 2;

    /* Final result */
    seq_t res = pow2 * poly;
    res /= 12;                      /* exact division by 12 */
    return res;
}
