/*
 * A058310  a(n) = (1/2)*(n^2+n+2)*(n^2+3*n+1).
 * https://oeis.org/A058310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^2 + n + 2) * (n^2 + 3*n + 1)) / 2
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t term1 = n2 + nn + (seq_t)2;          /* n^2 + n + 2  (even)   */
    seq_t term2 = n2 + (seq_t)3 * nn + (seq_t)1; /* n^2 + 3n + 1 (odd)    */
    seq_t prod  = term1 * term2;               /* product is even       */
    return prod / 2;                           /* integer division      */
}
