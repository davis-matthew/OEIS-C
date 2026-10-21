/*
 * A212514  Number of (w,x,y,z) with all terms in {1,...,n} and w<=2x and
 *          y>3z.
 * https://oeis.org/A212514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 431, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (∑_{x=1}^{n} min(n, 2x)) * (∑_{z=1}^{n} max(0, n - 3z))
 * Simplified:
 *   let p = n / 2
 *   left  = p*(p + 1) + n*(n - p)
 *   let q = (n - 1) / 3
 *   right = q*n - 3*q*(q + 1)/2
 *   a(n)  = left * right
 * Initial values: a(0)=a(1)=a(2)=a(3)=0.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* left side: number of pairs (w,x) with w<=2x */
    idx_t p = n / 2;
    seq_t left = (seq_t)p * (p + 1) + (seq_t)n * (n - p);

    /* right side: number of pairs (y,z) with y>3z */
    idx_t q = (n - 1) / 3;
    seq_t right = (seq_t)q * n;
    seq_t tmp = (seq_t)3 * (seq_t)q * (q + 1) / 2;
    right -= tmp;

    return left * right;
}
