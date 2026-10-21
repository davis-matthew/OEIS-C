/*
 * A211520  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w + 4y = 2x.
 * https://oeis.org/A211520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)^2/4) - floor((n-1)/4)*floor((n+1)/4); a(0) =
 *            0
 * Range:     u8 n <= 37, u16 n <= 591, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;
    seq_t term1 = (n1 * n1) / 4;                     /* floor((n-1)^2 / 4)   */
    seq_t term2 = (n1 / 4) * ((nn + 1) / 4);          /* floor((n-1)/4)*floor((n+1)/4) */
    return term1 - term2;
}
