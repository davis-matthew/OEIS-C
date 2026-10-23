/*
 * A134158  a(n) = 1 + 27n + 252n^2 + 882n^3 + 1029n^4.
 * https://oeis.org/A134158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 27*n + 252*n^2 + 882*n^3 + 1029*n^4 (OEIS A134158)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t r  = (seq_t)1029;
    r = r * nn + (seq_t)882;
    r = r * nn + (seq_t)252;
    r = r * nn + (seq_t)27;
    r = r * nn + (seq_t)1;
    return r;
}
