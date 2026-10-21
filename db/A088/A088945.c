/*
 * A088945  a(n) =60*sum(1<=u<=v<=w<=m,u^2*v^2/w).
 * https://oeis.org/A088945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(200*n^5+1752*n^4+5705*n^3+8370*n^2+5075*n+498)/360
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;          /* n^2 */
    seq_t n3 = n2 * nseq;            /* n^3 */
    seq_t n4 = n3 * nseq;            /* n^4 */
    seq_t n5 = n4 * nseq;            /* n^5 */

    seq_t poly = (seq_t)200 * n5
               + (seq_t)1752 * n4
               + (seq_t)5705 * n3
               + (seq_t)8370 * n2
               + (seq_t)5075 * nseq
               + (seq_t)498;

    seq_t numerator = nseq * poly;   /* n * poly */
    return numerator / (seq_t)360;   /* division is exact */
}
