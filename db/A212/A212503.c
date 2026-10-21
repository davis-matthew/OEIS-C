/*
 * A212503  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x and y<2z.
 * https://oeis.org/A212503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( floor((n+1)/2)^2 + n*(n - floor((n+1)/2)) )^2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077043(n) = floor((n+1)/2)^2 + n*(n - floor((n+1)/2))   */
/* a(n) = (A077043(n))^2                                       */

seq_t a(idx_t n)
{
    /* m = floor((n+1)/2) = ceil(n/2) for unsigned n */
    idx_t m = (n >> 1) + (n & 1);

    seq_t nm = (seq_t)n;
    seq_t mm = (seq_t)m;

    seq_t term1 = mm * mm;               /* m^2 */
    seq_t term2 = nm * (nm - mm);        /* n*(n - m) */

    seq_t a_n = term1 + term2;           /* A077043(n) */

    return a_n * a_n;                    /* a(n) = (A077043(n))^2 */
}
