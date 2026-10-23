/*
 * A100156  Structured truncated tetrahedral numbers.
 * https://oeis.org/A100156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (11*n^3 - 3*n^2 - 2*n)/6
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 1250, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type first to avoid overflow */
    seq_t nn   = (seq_t)n;
    seq_t nn2  = nn * nn;        /* n^2 */
    seq_t nn3  = nn2 * nn;       /* n^3 */
    seq_t numer = 11 * nn3   /* 11*n^3 */
                - 3 * nn2    /* 3*n^2 */
                - 2 * nn;    /* 2*n */
    return numer / 6;            /* exact division by 6 */
}
