/*
 * A213810  a(n) = 4*n^2 - 482*n + 14561.
 * https://oeis.org/A213810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 - 482*n + 14561
 * Range:     u8 none, u16 n <= 188, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;      /* n^2 */
    res = res * 4;            /* 4*n^2 */
    res = res - nn * 482;     /* subtract 482*n */
    res = res + 14561;        /* add constant term */
    return res;
}
