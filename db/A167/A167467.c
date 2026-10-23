/*
 * A167467  a(n) = 25*n^3 - n*(5*n+1)/2 + 1.
 * https://oeis.org/A167467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^3 - n*(5*n+1)/2 + 1
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 555, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n as seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */

    seq_t term1 = (seq_t)25 * n3;                                     /* 25*n^3 */
    seq_t term2 = nn * (((seq_t)5 * nn) + (seq_t)1) / (seq_t)2;      /* n*(5*n+1)/2 */

    return term1 - term2 + (seq_t)1;   /* 25*n^3 - n*(5*n+1)/2 + 1 */
}
