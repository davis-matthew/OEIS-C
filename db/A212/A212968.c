/*
 * A212968  Number of (w,x,y) with all terms in {0,...,n} and w>=range{w,x,y}.
 * https://oeis.org/A212968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(14*n*(n+2)+3*(-1)^n+21)/24
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to seq_t for arithmetic */
    /* Compute 14*n*(n+2) + 21 */
    seq_t base = (seq_t)14 * N * (N + (seq_t)2) + (seq_t)21;
    /* Add or subtract 3 depending on parity of n */
    seq_t part = ((n & (idx_t)1) == 0) ? (base + (seq_t)3)
                                        : (base - (seq_t)3);
    /* Numerator (n+1)*part */
    seq_t numer = (N + (seq_t)1) * part;
    /* Division by 24 (exact) */
    return numer / (seq_t)24;
}
