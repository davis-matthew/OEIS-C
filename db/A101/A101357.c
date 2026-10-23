/*
 * A101357  Duplicate of A004255.
 * https://oeis.org/A101357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ((n*(n+1)/2)^2 - 3*n*(n+1)*(2*n+1)/6 + 4*n*(n+1)/2)/2
 * Range:     i8 n <= 5, i16 n <= 19, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nst = (seq_t)n;
    seq_t t   = nst * (nst + (seq_t)1);          /* n*(n+1) */
    seq_t S1  = t >> 1;                         /* n*(n+1)/2 */
    seq_t S2  = t * ((nst << 1) + (seq_t)1) / (seq_t)6; /* n*(n+1)*(2n+1)/6 */
    seq_t S3  = S1 * S1;                         /* (n*(n+1)/2)^2 */
    seq_t num = S3 - ((seq_t)3) * S2 + ((seq_t)4) * S1;
    return num >> 1;                             /* divide by 2 */
}
