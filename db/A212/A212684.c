/*
 * A212684  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          |x-y|=n-w+|y-z|.
 * https://oeis.org/A212684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(n+2)*(2*n-1)-(-1)^n+1)/8
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212684 – Number of quadruples (w,x,y,z) in {1,…,n} satisfying |x-y| = n-w+|y-z| */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 2 * n * (n + 2) * (2*n - 1) */
    seq_t term = (seq_t)2 * nn * (nn + (seq_t)2) * ( (seq_t)2 * nn - (seq_t)1 );

    /* Add 0 if n is even, or 2 if n is odd, corresponding to -(-1)^n + 1 */
    seq_t add = ((n & (idx_t)1) == (idx_t)0) ? (seq_t)0 : (seq_t)2;
    term += add;

    /* Divide by 8 (constant) */
    return term / (seq_t)8;
}
