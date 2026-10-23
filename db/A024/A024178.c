/*
 * A024178  a(n) = floor((3rd elementary symmetric function of
 *          2,3,...,n+3)/(2+3+...+n+3)).
 * https://oeis.org/A024178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+1)*(n*n + 9*n + 22)/24)
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 327, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;                /* convert index to seq_t */
    seq_t t1  = nn * (nn + 1);          /* n*(n+1)                     */
    seq_t t2  = nn * nn + (seq_t)9 * nn + (seq_t)22; /* n^2 + 9*n + 22 */
    return (t1 * t2) / 24;              /* integer division gives floor */
}
