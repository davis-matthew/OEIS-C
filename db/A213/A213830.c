/*
 * A213830  Antidiagonal sums of the convolution array A213828.
 * https://oeis.org/A213830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + n^2 + 2*n^3 + 3*n^4)/4
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1.  For safety we return 0 for n = 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t nS = (seq_t)n;          /* cast index to seq_t for exact arithmetic  */
    seq_t n2 = nS * nS;           /* n^2          */
    seq_t n3 = n2 * nS;           /* n^3          */
    seq_t n4 = n3 * nS;           /* n^4          */

    /* Compute numerator: 2*n + n^2 + 2*n^3 + 3*n^4 */
    seq_t term = nS * (seq_t)2;   /* 2*n */
    term += n2;                   /* + n^2 */
    term += n3 * (seq_t)2;        /* + 2*n^3 */
    term += n4 * (seq_t)3;        /* + 3*n^4 */

    /* Final division by 4 (the result is always integer and even) */
    return term / (seq_t)4;
}
