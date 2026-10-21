/*
 * A228290  a(n) = n^6 + n^5 + n^4 + n^3 + n^2 + n.
 * https://oeis.org/A228290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + n^5 + n^4 + n^3 + n^2 + n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n */
    seq_t p2 = nn * nn;          /* n^2 */
    seq_t p3 = p2 * nn;          /* n^3 */
    seq_t p4 = p3 * nn;          /* n^4 */
    seq_t p5 = p4 * nn;          /* n^5 */
    seq_t p6 = p5 * nn;          /* n^6 */

    /* Sum all terms: n + n^2 + ... + n^6 */
    seq_t sum = nn;
    sum += p2;
    sum += p3;
    sum += p4;
    sum += p5;
    sum += p6;

    return sum;
}
