/*
 * A213554  Principal diagonal of the convolution array A213553.
 * https://oeis.org/A213554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(39*n^4 + 15*n^3 - 25*n^2 + 1)/30
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t to avoid implicit conversions on small types */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                /* n^2 */
    seq_t n3 = n2 * nn;                /* n^3 */
    seq_t n4 = n3 * nn;                /* n^4 */
    seq_t n5 = n4 * nn;                /* n^5 */

    seq_t term1 = (seq_t)39 * n4;       /* 39*n^4 */
    seq_t term2 = (seq_t)15 * n3;       /* 15*n^3 */
    seq_t term3 = (seq_t)25 * n2;       /* 25*n^2 */

    /* inner polynomial: 39*n^4 + 15*n^3 - 25*n^2 + 1 */
    seq_t inner = term1 + term2 - term3 + (seq_t)1;

    /* Multiply by n and divide by 30 */
    seq_t prod   = nn * inner;
    seq_t result = prod / (seq_t)30;

    return result;
}
