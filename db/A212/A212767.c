/*
 * A212767  Number of (w,x,y,z) with all terms in {0,...,n}, w even, x even,
 *          and w+x=y+z.
 * https://oeis.org/A212767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3+15*n^2+20*n+12+3*(n^2+4*n+4)*(-1)^n)/24
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* compute powers */
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* main term */
    seq_t term = (seq_t)4 * n3 + (seq_t)15 * n2 + (seq_t)20 * N + (seq_t)12;

    /* alternating part */
    seq_t alt = n2 + (seq_t)4 * N + (seq_t)4;
    seq_t alt3 = (seq_t)3 * alt;

    if ((n & (idx_t)1) == (idx_t)0) {
        term += alt3;          /* even n */
    } else {
        term -= alt3;          /* odd n */
    }

    /* divide by 24 */
    return term / (seq_t)24;
}
