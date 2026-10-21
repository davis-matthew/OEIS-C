/*
 * A051437  Number of undirected walks of length n+1 on an oriented triangle,
 *          visiting n+2 vertices, with n "corners"; the symmetry group is C3.
 *          Walks are not self-avoiding.
 * https://oeis.org/A051437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, nice, easy
 * Formula:   a(n) = 2*a(n-2) + 2^(n-1), with a(0)=1, a(1)=3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* iteratively compute using recurrence */
    seq_t prev2 = (seq_t)1;   /* a(n-2)  */
    seq_t prev1 = (seq_t)3;   /* a(n-1)  */
    seq_t pow2  = (seq_t)2;   /* 2^(n-1) for n=2 */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = prev2 * 2 + pow2;   /* a(i) = 2*a(i-2) + 2^(i-1) */
        /* prepare for next iteration */
        pow2  = pow2 * 2;            /* next power of two */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
