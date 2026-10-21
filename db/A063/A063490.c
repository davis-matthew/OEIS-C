/*
 * A063490  a(n) = (2*n - 1)*(7*n^2 - 7*n + 6)/6.
 * https://oeis.org/A063490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1)*(7*n^2 - 7*n + 6)/6
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 706, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the element type */
    seq_t nseq = (seq_t)n;

    /* Compute each factor using only seq_t arithmetic */
    seq_t first  = 2*nseq - 1;                         /* 2*n - 1 */
    seq_t second = 7*nseq*nseq - 7*nseq + 6;          /* 7*n^2 - 7*n + 6 */

    /* Final result, division truncates as required */
    return (first * second) / 6;
}
