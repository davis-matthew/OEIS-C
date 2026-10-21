/*
 * A087645  Third column of A071223.
 * https://oeis.org/A087645
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4-10*n^3+9*n^2-2*n+24)/12
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 219, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2; otherwise return 0. */
    if (n < 2)
        return (seq_t)0;

    seq_t tn   = (seq_t)n;
    seq_t tn2  = tn * tn;          /* n^2  */
    seq_t tn3  = tn2 * tn;         /* n^3  */
    seq_t tn4  = tn3 * tn;         /* n^4  */

    /* Compute numerator as a sum of positive terms minus a sum of positive terms
       to avoid intermediate wraparound. */
    seq_t pos   = (seq_t)3 * tn4 + (seq_t)9 * tn2 + (seq_t)24;
    seq_t neg   = (seq_t)10 * tn3 + (seq_t)2 * tn;
    seq_t num   = pos - neg;       /* 3*n^4 - 10*n^3 + 9*n^2 - 2*n + 24 */

    /* Divide by 12. The division is exact for all n >= 2. */
    return num / (seq_t)12;
}
