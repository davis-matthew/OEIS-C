/*
 * A212520  Number of (w,x,y,z) with all terms in {1,...,n} and w>=2x and
 *          y<3z.
 * https://oeis.org/A212520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A(n)*B(n), where A(n) = floor(n/2) * (n - floor(n/2))
 *            B(n) = floor((n+1)/3) * (3*floor((n+1)/3)+1)/2 + (n -
 *            floor((n+1)/3)) * n
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 378, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute A(n) = floor(n/2) * (n - floor(n/2)) */
    idx_t m = n / 2;                    /* floor(n/2) */
    seq_t A = (seq_t)m * ((seq_t)n - (seq_t)m);

    /* Compute B(n) */
    idx_t t = (n + 1) / 3;              /* floor((n+1)/3) */
    seq_t sum1 = (seq_t)t * (((seq_t)3 * (seq_t)t) + 1) / 2; /* t*(3t+1)/2 */
    seq_t sum2 = (seq_t)(n - t) * (seq_t)n;                 /* (n-t)*n */
    seq_t B = sum1 + sum2;

    return A * B;
}
