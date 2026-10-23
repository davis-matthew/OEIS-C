/*
 * A034721  a(n) = (10*n^3 - 9*n^2 + 2*n)/3 + 1.
 * https://oeis.org/A034721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^3 - 9*n^2 + 2*n)/3 + 1 (OEIS A034721)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 865, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to sequence type for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */

    /* Compute 10*n^3 - 9*n^2 + 2*n */
    seq_t term = (seq_t)10 * n3;  /* 10*n^3 */
    term -= (seq_t)9 * n2;        /* -9*n^2 */
    term += (seq_t)2 * nn;        /* +2*n */

    /* Divide by 3 and add 1 */
    term /= (seq_t)3;
    term += (seq_t)1;

    return term;
}
