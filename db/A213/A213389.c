/*
 * A213389  Number of (w,x,y) with all terms in {0,...,n} and max(w,x,y) <
 *          2*min(w,x,y).
 * https://oeis.org/A213389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*ceil(n²/4) = n*((n*n+3)/4)
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* convert index to seq_t for safe math   */
    seq_t sq = nn * nn;           /* n², safe as seq_t is big/64-bit       */
    seq_t ceil_div = (sq + (seq_t)3) / (seq_t)4; /* ceil division by 4   */
    return nn * ceil_div;         /* final result, n * ceil(n²/4)          */
}
